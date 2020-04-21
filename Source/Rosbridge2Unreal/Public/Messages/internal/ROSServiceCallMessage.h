// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeMessage.h"
#include "ROSServiceCallMessage.generated.h"

UCLASS()
class ROSBRIDGE2UNREAL_API UROSServiceCallMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Data */
	ROSData Data;

	/* Request Data */
	FString ID;
	FString ServiceName;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};