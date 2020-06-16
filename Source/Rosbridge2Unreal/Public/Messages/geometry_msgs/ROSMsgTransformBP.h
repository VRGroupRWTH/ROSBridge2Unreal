// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgTransformBP.generated.h"

/**
 *  Low Precision variant of the UROSMsgQuaternion, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgTransformBP : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTransformBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Transform";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformBP* CreateFromTransform(const FTransform& Transform);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformBP* CreateFromTranslationRotation(const FVector& Translation, const FQuat& Rotation);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformBP* Create(const float& Tx,const float& Ty,const float& Tz,const float& Rx,const float& Ry,const float& Rz,const float& Rw);

	/* Ease of use */
	UFUNCTION(BlueprintCallable, BlueprintPure) FTransform AsTransform() const;
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Translation;							
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuat Rotation;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};