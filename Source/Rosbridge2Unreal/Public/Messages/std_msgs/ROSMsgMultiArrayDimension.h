// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSMessageBase.h"
#include "ROSMsgMultiArrayDimension.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgMultiArrayDimension : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgMultiArrayDimension(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/MultiArrayDimension";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgMultiArrayDimension* Create(const FString Label, const int64 Size, const int64 Stride);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Label; /* Label of given Dimension */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 Size; /* Size of given Dimension */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 Stride; /* Stride of given Dimension */

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
