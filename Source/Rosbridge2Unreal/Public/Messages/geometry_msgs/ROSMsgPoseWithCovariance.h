#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/geometry_msgs/ROSMsgPose.h"
#include "ROSMsgPoseWithCovariance.generated.h"

/**
 * Since TArray<double> is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgPoseWithCovariance : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgPoseWithCovariance(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/PoseWithCovariance";};
	static UROSMsgPoseWithCovariance* Create(UROSMsgPose* Pose, const TArray<double>& Covariance);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseWithCovariance* CreateEmpty();
	
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
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
