// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSMessageBase.h"
#include "ROSMsgMultiArrayDimension.h"
#include "ROSMsgMultiArrayLayout.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgMultiArrayLayout : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgMultiArrayLayout(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/MultiArrayLayout";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgMultiArrayLayout* CreateEmpty();
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgMultiArrayLayout* Create(TArray<UROSMsgMultiArrayDimension*>& Dimensions, const int64& DataOffset);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UROSMsgMultiArrayDimension*> Dimensions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 DataOffset; /* Padding elements in front of the data */

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
