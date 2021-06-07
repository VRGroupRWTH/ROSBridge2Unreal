// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Rosbridge2Unreal.h"
#include "Core.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "FRosbridge2UnrealModule"

void FRosbridge2UnrealModule::StartupModule()
{	
	FWorldDelegates::OnWorldCleanup.AddRaw(this, &FRosbridge2UnrealModule::OnSessionEnd);

	FWorldDelegates::OnWorldTickStart.AddRaw(this, &FRosbridge2UnrealModule::OnWorldTickStart);
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
	
	if (RosBridge)
	{
		RosBridge->Uninitialize();
		RosBridge->RemoveFromRoot(); /* remove the object from the root set to allow garbage collection */
		RosBridge = nullptr;
	}
}

#if ENGINE_MINOR_VERSION > 23
void FRosbridge2UnrealModule::OnWorldTickStart(UWorld*, ELevelTick TickType, float DeltaTime)
#else 
void FRosbridge2UnrealModule::OnWorldTickStart(ELevelTick TickType, float DeltaTime)
#endif
{
	if (TickType != ELevelTick::LEVELTICK_TimeOnly) return; // Nothing to do here

	if (RosBridge) RosBridge->TickEvent(DeltaTime);
}

bool FRosbridge2UnrealModule::InitializeConnection()
{
	// Only try to intitialize the connection once.
	// For every subsequent call only return whether we are connected.
	if (RosBridge == nullptr)
	{
		RosBridge = NewObject<UROSBridge>();
		RosBridge->AddToRoot();
		return RosBridge->Initialize();
	}
	else
	{
		return RosBridge->IsInitialized();
	}
}

bool FRosbridge2UnrealModule::IsConnected()
{
	return RosBridge != nullptr && RosBridge->IsInitialized();
}

UROSBridge* FRosbridge2UnrealModule::GetBridge()
{
	return RosBridge;
}

bool FRosbridge2UnrealModule::IsBSONMode()
{
	if (!RosBridge) return false;
	return RosBridge->IsBSONMode();
}

bool FRosbridge2UnrealModule::SendMessage(const UROSBridgeMessage& Message)
{
	return InitializeConnection() ? RosBridge->SendMessage(Message) : false;
}

long FRosbridge2UnrealModule::GetNextID()
{
	return InitializeConnection() ? RosBridge->GetNextID() : -1;
}

UROSTopic* FRosbridge2UnrealModule::GetTopic(const FString& TopicName, TSubclassOf<UROSMessageBase> MessageClass)
{
	return InitializeConnection() ? RosBridge->GetTopic(TopicName, MessageClass) : nullptr;
}

UROSService* FRosbridge2UnrealModule::GetService(const FString& ServiceName, TSubclassOf<UROSServiceBase> ServiceClass)
{
	return InitializeConnection() ? RosBridge->GetService(ServiceName, ServiceClass) : nullptr;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRosbridge2UnrealModule, Rosbridge2Unreal)
