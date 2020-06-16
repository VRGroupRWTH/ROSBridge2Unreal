// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgPointBP.generated.h"

/**
 * Low Precision variant of the UROSMsgPoint, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPointBP : public UROSMessageBase
{
	GENERATED_BODY()
		
public:
	/* Construction */
	UROSMsgPointBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Point";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPointBP* CreateFromVector(const FVector& Data);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPointBP* Create(const float& X, const float& Y, const float& Z);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Data;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
