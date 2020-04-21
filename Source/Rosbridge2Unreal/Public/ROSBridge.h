// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ROSTopic.h"
#include "Socket/TCPConnection.h"
#include "HAL/Runnable.h"
#include "ROSService.h"
#include "ROSBridge.generated.h"

/**
 * Handles all communication with the ROSBridge on the other side.
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSBridge : public UObject, public FRunnable
{
	GENERATED_BODY()
public:

	bool Initialize(FString IPAddress, int Port, TransportMode Mode);
	void Uninitialize();
	
	bool SendMessage(const FString& Data) const;

	long GetNextID();

	bool IsBSONMode() const;

	bool SendMessage(UROSBridgeMessage &Message) const;

	/* Get or create an internal TopicHandle and return it */
	UROSTopic* GetTopic(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass);

	/* Get or create an internal ServiceHandle and return it */
	UROSService* GetService(FString ServiceName,  TSubclassOf<UROSServiceBase> ServiceClass);
	
private:
	/* Message Queueing */
	void QueueMessage(UROSBridgeMessage* Message);
	UPROPERTY() TArray<UROSBridgeMessage*> QueuedMessages;
	FRunnableThread* SenderThread = nullptr;
	uint32 Run() override;
	bool KeepSenderThreadRunning = true;

	/* The connection to the ROSBridge */
	UPROPERTY() UTCPConnection* Connection;

	/* All registered Topics and Services */
	UPROPERTY() TArray<UROSTopic*> Topics;
	UPROPERTY() TArray<UROSService*> Services;

	/* Used to generate increasing IDs for messages */
	long IDCounter = 0;

	/* Callbacks from TCPConnection */
	void IncomingMessage(const ROSData& Message);

	/* Critical Sections */
	FCriticalSection MutexMessageQueue;
};