// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgQuaternion.generated.h"

/**
 * Since 'double' is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgQuaternion : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgQuaternion(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Quaternion";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgQuaternion* Create(const FQuat& Data);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgQuaternion* CreateEmpty();

	/* Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FQuat AsFQuat() const;
	
	/* Data */
	double X;
	double Y;
	double Z;
	double W;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
