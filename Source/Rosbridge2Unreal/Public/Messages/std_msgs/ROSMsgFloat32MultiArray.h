// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSMessageBase.h"
#include "ROSMsgMultiArrayLayout.h"
#include "ROSMsgFloat32MultiArray.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgFloat32MultiArray : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgFloat32MultiArray(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/Float32MultiArray";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgFloat32MultiArray* Create(TArray<float>& Data, UROSMsgMultiArrayLayout* Layout);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgFloat32MultiArray* CreateWithEmptyLayout(TArray<float>& Data);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgMultiArrayLayout* Layout = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Data;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
