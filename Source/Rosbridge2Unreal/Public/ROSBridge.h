#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ROSTopic.h"
#include "Socket/TCPConnection.h"
#include "HAL/Runnable.h"
#include "ROSService.h"
#include "Messages/graph_msgs/ROSMsgClock.h"
#include "RosbridgeSettings.h"
#include "ROSBridge.generated.h"

/**
 * Handles all communication with the ROSBridge on the other side.
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSBridge : public UObject, public FRunnable
{
	GENERATED_BODY()
public:

	/**
	 * Reads settings from project settings and initializes the connection
	 * @return Connection successful
	 */
	bool Initialize();
	
	/**
	 * Uninitialize the bridge. Disconnects and destroys all held topics and services.
	 */
	void Uninitialize();
	
	/**
	 * Returns whether the connection is initalized.
	 * @return Connection successful 
	 */
	inline bool IsInitialized() const { return bInitialized; }
	
	/**
	 * Send a String to the ROS Bridge
	 * @param Data - The String to send
	 * @return Success of sending operation
	 */
	bool SendMessage(const FString& Data) const;

	/**
	 * Generated one time IDs that can be used in successive messages
	 * @return Successive one time IDs
	 */
	long GetNextID();

	/**
	 * @return Check if connection is in BSON mode
	 */
	bool IsBSONMode() const;

	/**
	 * Serialize and send message to the ROS Bridge
	 * @param Message - The message to send
	 * @return Success of sending operation
	 */
	bool SendMessage(const UROSBridgeMessage &Message) const;

	/**
	 * Retrieves a topic instance from the already registered ones. Creates a new one of not present so far.
	 * @param TopicName - The topic name that is used for advertise/subscribe etc.
	 * @param MessageClass - The message class that is used for (de-)serialization of messages
	 * @return The topic instance
	 */
	UROSTopic* GetTopic(const FString& TopicName, TSubclassOf<UROSMessageBase> MessageClass);

	/**
	 * Retrieves a service instance from the already registered ones. Creates a new one of not present so far.
	 * @param ServiceName - The service name that is used for advertise/call etc.
	 * @param ServiceClass - The service class that is used for (de-)serialization of requests/responses
	 * @return The service instance
	 */
	UROSService* GetService(const FString& ServiceName, TSubclassOf<UROSServiceBase> ServiceClass);

	/**
	 * Used internally to send clock events
	 */
	void TickEvent(float DeltaTime);
	
private:
	/* Message Queueing */
	void QueueMessage(UROSBridgeMessage* Message);
	UPROPERTY() TArray<UROSBridgeMessage*> QueuedMessages;
	FRunnableThread* SenderThread = nullptr;
	uint32 Run() override;
	bool KeepSenderThreadRunning = true;

	/* The connection to the ROSBridge */
	UPROPERTY() UNetworkConnection* Connection = nullptr;

	/* All registered Topics and Services */
	UPROPERTY() TArray<UROSTopic*> Topics;
	UPROPERTY() TArray<UROSService*> Services;

	/* Used to generate increasing IDs for messages */
	long IDCounter = 0;

	/* Callbacks from TCPConnection */
	void IncomingMessage(const ROSData& Message);

	/* Critical Sections */
	FCriticalSection MutexMessageQueue;

	/* Timing Stuff */
	UPROPERTY() UROSTopic* ClockTopic = nullptr;
	UPROPERTY() UROSMsgClock* ClockMessage;
	bool bSetUpdateIntervalSettings = false;

	/* Settings */
	UPROPERTY() const URosbridgeSettings* Settings;
	bool bSettingsRead = false;

	/* Status */
	bool bInitialized = false;
};
