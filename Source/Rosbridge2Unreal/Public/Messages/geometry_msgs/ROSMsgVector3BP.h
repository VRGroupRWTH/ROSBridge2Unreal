// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgVector3BP.generated.h"

/**
 *   Low Precision variant of the UROSMsgVector3, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgVector3BP : public UROSMessageBase
{
	GENERATED_BODY()
		
public:
	/* Construction */
	UROSMsgVector3BP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Vector3";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgVector3BP* CreateFromVector(const FVector& Data);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgVector3BP* Create(const float& X, const float& Y, const float& Z);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Vector;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
