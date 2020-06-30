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
	
	if(RosBridge){
		RosBridge->Uninitialize();
		RosBridge->RemoveFromRoot(); /* remove the object from the root set to allow garbage collection */
		RosBridge = nullptr;
	}
	bRosBridgeInitialized = false;
}

#if ENGINE_MINOR_VERSION > 23
void FRosbridge2UnrealModule::OnWorldTickStart(UWorld*, ELevelTick TickType, float DeltaTime)
#else 
void FRosbridge2UnrealModule::OnWorldTickStart(ELevelTick TickType, float DeltaTime)
#endif
{
	if(TickType != ELevelTick::LEVELTICK_TimeOnly) return; // Nothing to do here

	if(RosBridge) RosBridge->TickEvent(DeltaTime);
}

void FRosbridge2UnrealModule::InitializeConnection()
{
	if(bRosBridgeInitialized) return;
		
	RosBridge = NewObject<UROSBridge>();
	RosBridge->AddToRoot(); /* Disallow Garbage Collection for this UObject */
	bRosBridgeInitialized = RosBridge->Initialize();
}

void FRosbridge2UnrealModule::EnsureConnectionIsInitialized()
{
	if(!bRosBridgeInitialized) InitializeConnection();	
}

UROSBridge* FRosbridge2UnrealModule::GetBridge()
{
	return RosBridge;
}

bool FRosbridge2UnrealModule::IsBSONMode()
{
	if(!RosBridge) return false;
	return RosBridge->IsBSONMode();
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

UROSTopic* FRosbridge2UnrealModule::GetTopic(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass)
{
	EnsureConnectionIsInitialized();
	return RosBridge->GetTopic(TopicName, MessageClass);
}

UROSService* FRosbridge2UnrealModule::GetService(FString ServiceName, TSubclassOf<UROSServiceBase> ServiceClass)
{
	EnsureConnectionIsInitialized();
	return RosBridge->GetService(ServiceName, ServiceClass);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRosbridge2UnrealModule, Rosbridge2Unreal)
