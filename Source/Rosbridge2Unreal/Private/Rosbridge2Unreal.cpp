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
	
	bConnectionFailed = false; //Allow new attempt
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRosbridge2UnrealModule, Rosbridge2Unreal)
