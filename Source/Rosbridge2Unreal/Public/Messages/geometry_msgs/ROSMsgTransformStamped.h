// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgHeader.h"
#include "ROSMsgTransform.h"
#include "ROSMsgTransformStamped.generated.h"

/**
 * Since 'double' is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgTransformStamped : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTransformStamped(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/TransformStamped";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformStamped* Create(UROSMsgHeader* Header, const FString& ChildFrameID, UROSMsgTransform* Transform);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformStamped* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgTransform* Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ChildFrameID;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
