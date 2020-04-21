// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeMessage.h"
#include "ROSTopicSubscribeMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSTopicSubscribeMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:	
	/* Data */
	FString ID;
	FString TopicName;
	FString MessageType;
	FString Compression = "none";
	int ThrottleRate = 0;
	int QueueLength = 10;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
