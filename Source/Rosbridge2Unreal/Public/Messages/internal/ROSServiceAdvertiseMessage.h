// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSServiceAdvertiseMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSServiceAdvertiseMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Request Data */
	FString ServiceName;
	FString ServiceType;
	FString ID;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
