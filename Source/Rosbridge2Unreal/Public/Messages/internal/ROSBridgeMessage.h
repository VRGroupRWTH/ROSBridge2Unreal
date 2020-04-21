// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"

#include "ROSBridgeMessage.generated.h"


UCLASS(abstract, hidedropdown)
class ROSBRIDGE2UNREAL_API UROSBridgeMessage : public UObject
{
	GENERATED_BODY()

public:
	virtual void ToData(ROSData& Message) const PURE_VIRTUAL(UROSBridgeMessage::ToData, return;);
	virtual bool FromData(const ROSData& Message) PURE_VIRTUAL(UROSBridgeMessage::FromData, return false;);
};

