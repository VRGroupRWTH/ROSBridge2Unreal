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

    virtual bool HasFailedConnection() const override { return bConnectionFailed;} 
    virtual void ReportFailedConnection() override { bConnectionFailed = true; }

private:
	bool bConnectionFailed = false;
};
