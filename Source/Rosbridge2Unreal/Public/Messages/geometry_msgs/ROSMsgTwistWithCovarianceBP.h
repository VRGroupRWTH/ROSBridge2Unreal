// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/geometry_msgs/ROSMsgTwistBP.h"
#include "ROSMsgTwistWithCovarianceBP.generated.h"

/**
 * Low Precision variant of the UROSMsgTwistWithCovariance, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgTwistWithCovarianceBP : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTwistWithCovarianceBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/TwistWithCovariance";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwistWithCovarianceBP* Create(UROSMsgTwistBP* Twist, const TArray<float>& Covariance);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwistWithCovarianceBP* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgTwistBP* Twist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Covariance;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
