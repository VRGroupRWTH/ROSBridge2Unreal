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

	/* Reads settings from project settings and initializes the connection */
	bool Initialize();
	
	void Uninitialize();
	
	bool SendMessage(const FString& Data) const;

	long GetNextID();

	bool IsBSONMode() const;

	bool SendMessage(UROSBridgeMessage &Message) const;

	/* Get or create an internal TopicHandle and return it */
	UROSTopic* GetTopic(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass);

	/* Get or create an internal ServiceHandle and return it */
	UROSService* GetService(FString ServiceName,  TSubclassOf<UROSServiceBase> ServiceClass);

	/* Used to send clock events */
	void TickEvent(float DeltaTime);
	
private:
	/* Message Queueing */
	void QueueMessage(UROSBridgeMessage* Message);
	UPROPERTY() TArray<UROSBridgeMessage*> QueuedMessages;
	FRunnableThread* SenderThread = nullptr;
	uint32 Run() override;
	bool KeepSenderThreadRunning = true;

	/* The connection to the ROSBridge */
	UPROPERTY() UTCPConnection* Connection = nullptr;

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
