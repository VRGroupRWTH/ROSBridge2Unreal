#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgPoint.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPoint : public UROSMessageBase
{
	GENERATED_BODY()
		
public:
	/* Construction */
	UROSMsgPoint(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Point";};
	UFUNCTION(BlueprintCallable) static UROSMsgPoint* CreateFromVector(const FVector& Data = FVector::ZeroVector);
	static UROSMsgPoint* Create(const double& X, const double& Y, const double& Z);

	/* Blueprint functions. Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector AsFVector() const;
	UFUNCTION(BlueprintCallable) void SetFromFVector(const FVector InVector);

	/* Data */
	double X;
	double Y;
	double Z;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
