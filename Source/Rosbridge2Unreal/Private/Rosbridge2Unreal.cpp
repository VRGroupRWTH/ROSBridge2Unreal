// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Rosbridge2Unreal.h"
#include "Core.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "FRosbridge2UnrealModule"

void FRosbridge2UnrealModule::StartupModule()
{	
	OnWorldCleanupDelegate.BindRaw(this, &FRosbridge2UnrealModule::OnSessionEnd);
	FWorldDelegates::OnWorldCleanup.Add(OnWorldCleanupDelegate);
}

void FRosbridge2UnrealModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

/* Used as a destructor for all objects in this class */
void FRosbridge2UnrealModule::OnSessionEnd(UWorld* World, bool bSessionEnded, bool)
{
	if (!World->IsGameWorld() || !bSessionEnded) return;

	if(RosBridge){
		RosBridge->Uninitialize();
		RosBridge = nullptr;
	}
	bRosBridgeInitialized = false;
	bSettingsInitialized = false;
}

void FRosbridge2UnrealModule::InitializeConnection()
{
	if(bRosBridgeInitialized) return;
	
	EnsureConnection();
}

void FRosbridge2UnrealModule::InitializeConnection(FString InIPAddress, int InPort, bool InBSONMode)
{
	if(bRosBridgeInitialized) return;
	
	IPAddress = InIPAddress;
	Port = InPort;
	TransportMode = (InBSONMode) ? TransportMode::BSON : TransportMode::JSON;

	bSettingsInitialized = true;
	EnsureConnection();
}

void FRosbridge2UnrealModule::EnsureConnection()
{
	if(bRosBridgeInitialized) return;

	if(!bSettingsInitialized) ReadSettingsFromConfig();
	
	RosBridge = NewObject<UROSBridge>();
	bRosBridgeInitialized = RosBridge->Initialize(IPAddress, this->Port, TransportMode);
}

UROSBridge* FRosbridge2UnrealModule::GetBridge()
{
	return RosBridge;
}

bool FRosbridge2UnrealModule::IsBSONMode()
{
	return TransportMode == TransportMode::BSON;
}

bool FRosbridge2UnrealModule::SendMessage(UROSBridgeMessage& Message)
{
	if(!RosBridge) return false;
	return RosBridge->SendMessage(Message);
}

long FRosbridge2UnrealModule::GetNextID()
{
	if(!RosBridge) return -1;
	return RosBridge->GetNextID();
}

void FRosbridge2UnrealModule::ReadSettingsFromConfig()
{
	const URosbridgeSettings* Settings = GetDefault<URosbridgeSettings>();
	this->IPAddress = Settings->IP;
	this->Port = Settings->Port;
	this->TransportMode = Settings->TransportationMode;
	bSettingsInitialized = true;
}

UROSTopic* FRosbridge2UnrealModule::GetTopic(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass)
{
	EnsureConnection();
	return RosBridge->GetTopic(TopicName, MessageClass);
}

UROSService* FRosbridge2UnrealModule::GetService(FString ServiceName, TSubclassOf<UROSServiceBase> ServiceClass)
{
	EnsureConnection();
	return RosBridge->GetService(ServiceName, ServiceClass);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRosbridge2UnrealModule, Rosbridge2Unreal)
