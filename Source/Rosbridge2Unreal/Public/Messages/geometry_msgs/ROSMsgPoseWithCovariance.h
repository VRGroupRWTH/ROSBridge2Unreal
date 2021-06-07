#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/geometry_msgs/ROSMsgPose.h"
#include "ROSMsgPoseWithCovariance.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPoseWithCovariance : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgPoseWithCovariance(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/PoseWithCovariance";}
	static UROSMsgPoseWithCovariance* Create(UROSMsgPose* Pose, const TArray<double>& Covariance);
	UFUNCTION(BlueprintCallable) static UROSMsgPoseWithCovariance* Create(UROSMsgPose* Pose, const TArray<float>& Covariance);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseWithCovariance* CreateEmpty();

	/* Blueprint functions. Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable) TArray<float> CovarianceAsFloatArray() const;
	UFUNCTION(BlueprintCallable) void SetCovarianceFromFloatArray(const TArray<float>& InArray);
	
	/* Data */
	/*
		Row-major representation of the 6x6 covariance matrix
		The orientation parameters use a fixed-axis representation.
		In order, the parameters are:
		(x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgPose* Pose;
	TArray<double> Covariance;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
