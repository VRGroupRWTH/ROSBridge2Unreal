// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgTwist.generated.h"

/**
 * Since 'double' is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgTwist : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTwist(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Twist";};
	static UROSMsgTwist* Create(const double& Lx,const double& Ly,const double& Lz,const double& Ax,const double& Ay,const double& Az);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwist* CreateEmpty();

	/* Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector LinearAsFVector() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector AngularAsFVector() const;
	
	/* Data */
	double LinearX;
	double LinearY;
	double LinearZ;
	double AngularX;
	double AngularY;
	double AngularZ;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
