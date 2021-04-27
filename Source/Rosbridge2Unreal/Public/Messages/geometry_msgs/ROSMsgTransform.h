#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgTransform.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgTransform : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTransform(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Transform";}
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransform* CreateFromTransform(const FTransform& Transform);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransform* CreateFromTranslationRotation(const FVector& Translation, const FQuat& Rotation);
	static UROSMsgTransform* Create(double Tx, double Ty, double Tz, double Rx, double Ry, double Rz, double Rw);

	/* Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector TranslationAsFVector() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) FQuat RotationAsQuad() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) FTransform AsTransform() const;
	UFUNCTION(BlueprintCallable) void SetTranslationFromFVector(const FVector& InVector);
	UFUNCTION(BlueprintCallable) void SetRotationFromQuad(const FQuat& InQuat);
	UFUNCTION(BlueprintCallable) void SetFromTransform(const FTransform InTransform);
	
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
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};