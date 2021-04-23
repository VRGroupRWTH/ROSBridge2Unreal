#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/geometry_msgs/ROSMsgTwist.h"
#include "ROSMsgTwistWithCovariance.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgTwistWithCovariance : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTwistWithCovariance(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/TwistWithCovariance";};
	static UROSMsgTwistWithCovariance* Create(UROSMsgTwist* Twist, const TArray<double>& Covariance);
	UFUNCTION(BlueprintCallable) static UROSMsgTwistWithCovariance* Create(UROSMsgTwist* Twist, const TArray<float>& Covariance);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwistWithCovariance* CreateEmpty();

	/* Blueprint functions. Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable) TArray<float> CovarianceAsFloatArray() const;
	UFUNCTION(BlueprintCallable) void SetCovarianceFromFloatArray(const TArray<float> InArray);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgTwist* Twist;
	TArray<double> Covariance;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
