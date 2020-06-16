// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSMessageBase.h"
#include "ROSMsgString.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgString : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgString(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/String";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgString* Create(FString Data);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Data;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
