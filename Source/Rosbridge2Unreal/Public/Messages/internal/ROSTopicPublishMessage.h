// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeMessage.h"
#include "ROSTopicPublishMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSTopicPublishMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Data */
	ROSData Data;
	
	/* Request Data */
	FString ID;
	FString TopicName;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
