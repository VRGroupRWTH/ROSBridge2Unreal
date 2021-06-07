#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"
#include "UObject/NoExportTypes.h"
#include "HAL/Runnable.h"
#include "Socket/NetworkConnection.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSBridgeConnection.generated.h"

/**
 * Handles all communication with the ROSBridge on the other side.
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSBridgeConnection : public UActorComponent, public FRunnable
{
	GENERATED_BODY()
public:

	/**
	 * Reads settings from project settings and initializes the connection
	 * @return Connection successful
	 */
	virtual bool InitializeConnection();
	
	/**
	 * Uninitialize the bridge. Disconnects and destroys all held topics and services.
	 */
	virtual void UninitializeConnection();
	
	/**
	 * Returns whether the connection is initalized.
	 * @return Connection successful 
	 */
	inline bool IsConnectionInitialized() const { return bInitialized; }
	
	/**
	 * Serialize and send message to the ROS Bridge
	 * @param Message - The message to send
	 * @return Success of sending operation
	 */
	bool SendMessage(const UROSBridgeMessage &Message) const;

	/**
	 * Generated one time IDs that can be used in successive messages
	 * @return Successive one time IDs
	 */
	long GetNextID();
	
protected:

	/* Checks if message belongs to us */
	virtual bool HandleMessage(const FString& OPCode, const ROSData& Message) PURE_VIRTUAL(UROSBridgeConnection::HandleMessage, return false;);

	/* Message Queueing */
	void QueueMessage(UROSBridgeMessage* Message);
	UPROPERTY() TArray<UROSBridgeMessage*> QueuedMessages;
	FRunnableThread* SenderThread = nullptr;
	uint32 Run() override;
	bool KeepSenderThreadRunning = true;

	/* Critical Sections */
	FCriticalSection MutexMessageQueue;
	
	/* The connection to the ROSBridge */
	UPROPERTY() UNetworkConnection* Connection = nullptr;

	/* Fragmented Messages */
	TMap<FString,TArray<FString>> MessageFragments;
	
	/* Callbacks from TCPConnection */
	void IncomingMessage(const ROSData& Message);

	/* Status */
	bool bInitialized = false;

	/* Used to generate increasing IDs for messages */
	long IDCounter = 0;
};
