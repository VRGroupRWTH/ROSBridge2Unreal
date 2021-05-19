// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"
#include "IRosbridge2Unreal.h"
#include "Socket/TCPConnection.h"
#include "ROSTopic.h"
#include "Messages/ROSMessageBase.h"
#include "ROSClockEmitter.h"


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
	
private:

	/* Internal Bridge Network */
	UROSClockEmitter* ClockEmitter = nullptr;
};
