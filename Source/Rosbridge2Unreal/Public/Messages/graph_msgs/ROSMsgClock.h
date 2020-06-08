// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSMessageBase.h"
#include "ROSMsgClock.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSMsgClock : public UROSMessageBase
{
	GENERATED_BODY()

public:
	FString GetMessageType() override {return "rosgraph_msgs/Clock";};

	void BeginDestroy() override
	{
		Super::BeginDestroy();
		UE_LOG(LogTemp, Warning, TEXT("ClockMessageDestroyed"));
	};
	
	/* Convenience Methods */
	static UROSMsgClock* Create(int32 InSeconds, int32 InNanoSeconds);
	
	/* Request Data */
	int32 Seconds;
	int32 NanoSeconds;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
