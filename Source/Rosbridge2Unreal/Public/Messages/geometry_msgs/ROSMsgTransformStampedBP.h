// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "Messages/geometry_msgs/ROSMsgTransformBP.h"
#include "ROSMsgTransformStampedBP.generated.h"

/**
 * Low Precision variant of the UROSMsgQuaternion, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgTransformStampedBP : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTransformStampedBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/TransformStamped";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformStampedBP* Create(UROSMsgHeader* Header, const FString& ChildFrameID, UROSMsgTransformBP* Transform);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformStampedBP* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgTransformBP* Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ChildFrameID;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
