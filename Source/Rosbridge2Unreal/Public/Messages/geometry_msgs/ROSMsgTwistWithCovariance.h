// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/geometry_msgs/ROSMsgTwist.h"
#include "ROSMsgTwistWithCovariance.generated.h"

/**
 * Since 'double' is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgTwistWithCovariance : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTwistWithCovariance(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/TwistWithCovariance";};
	static UROSMsgTwistWithCovariance* Create(UROSMsgTwist* Twist, const TArray<double>& Covariance);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwistWithCovariance* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgTwist* Twist;
	TArray<double> Covariance;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
