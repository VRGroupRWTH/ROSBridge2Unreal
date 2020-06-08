// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"
#include "IRosbridge2Unreal.h"
#include "Modules/ModuleManager.h"
#include "RosbridgeSettings.h"
#include "Socket/TCPConnection.h"
#include "ROSTopic.h"
#include "ROSMessageBase.h"


class FRosbridge2UnrealModule : public IRosbridge2Unreal
{
	/* ROSBridge */
public:

	/* IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void OnSessionEnd(UWorld* World, bool bSessionEnded, bool);

	/* This Declaration changed over time */
	#if ENGINE_MINOR_VERSION > 23
		void OnWorldTickStart(UWorld * World, ELevelTick TickType, float DeltaTime);
	#else 
		void OnWorldTickStart(ELevelTick TickType, float DeltaTime);
	#endif
	
	/* Initializes with Settings in Project */
	void InitializeConnection() override;

	/* Ensure connection is made */
	void EnsureConnectionIsInitialized() override;

	/* Getter for the rosbridge2cpp::ROSBridge */
	UROSBridge* GetBridge() override;

	/* True if BSON is used, false if JSON */
	bool IsBSONMode() override;

	/* Send an internal Message to the other side */
	bool SendMessage(UROSBridgeMessage& Message) override;

	/* IDs used for internal sending */
	long GetNextID() override;
	
private:

	/* Internal Bridge Network */
	UROSBridge* RosBridge = nullptr;
	bool bRosBridgeInitialized = false;
	
 /* ROSTopics and Services */
public:
	/* Get or create an internal TopicHandle and return it */
	UROSTopic* GetTopic(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass) override;

	/* Get or create an internal ServiceHandle and return it */
	UROSService* GetService(FString ServiceName,  TSubclassOf<UROSServiceBase> ServiceClass) override;
};
