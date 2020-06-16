// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgTransform.generated.h"

/**
 * Since 'double' is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgTransform : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTransform(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Transform";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransform* CreateFromTransform(const FTransform& Transform);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransform* CreateFromTranslationRotation(const FVector& Translation, const FQuat& Rotation);
	static UROSMsgTransform* Create(const double& Tx,const double& Ty,const double& Tz,const double& Rx,const double& Ry,const double& Rz,const double& Rw);

	/* Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector TranslationAsFVector() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) FQuat RotationAsQuad() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) FTransform AsTransform() const;
	
	/* Data */
	/* Translation */
	double Tx;
	double Ty;
	double Tz;
	/* Rotation */
	double Rx;
	double Ry;
	double Rz;
	double Rw;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};