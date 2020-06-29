// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "Messages/geometry_msgs/ROSMsgPose.h"
#include "ROSMsgPoseStamped.generated.h"

/**
 * Since 'double' is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgPoseStamped : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgPoseStamped(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/PoseStamped";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseStamped* Create(UROSMsgHeader* Header, UROSMsgPose* Pose);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseStamped* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgPose* Pose;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
