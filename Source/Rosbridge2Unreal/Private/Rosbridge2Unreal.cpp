// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Rosbridge2Unreal.h"
#include "Core.h"
#include "Engine/World.h"
#include "ROSClockEmitter.h"

#define LOCTEXT_NAMESPACE "FRosbridge2UnrealModule"

DEFINE_LOG_CATEGORY(LogROSBridge);

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
	
	if(ClockEmitter)
	{
		ClockEmitter->UninitializeConnection();
		ClockEmitter->RemoveFromRoot(); /* remove the object from the root set to allow garbage collection */
		ClockEmitter = nullptr;
	}
}

#if ENGINE_MINOR_VERSION > 23
void FRosbridge2UnrealModule::OnWorldTickStart(UWorld*, ELevelTick TickType, float DeltaTime)
#else 
void FRosbridge2UnrealModule::OnWorldTickStart(ELevelTick TickType, float DeltaTime)
#endif
{
	if (TickType != ELevelTick::LEVELTICK_TimeOnly) return; // Nothing to do here

	if(GetSettings()->bEmitClockEvents){
		if (ClockEmitter == nullptr)
		{
			ClockEmitter = NewObject<UROSClockEmitter>();
			ClockEmitter->AddToRoot();
		}
		ClockEmitter->TickEvent(DeltaTime);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRosbridge2UnrealModule, Rosbridge2Unreal)
