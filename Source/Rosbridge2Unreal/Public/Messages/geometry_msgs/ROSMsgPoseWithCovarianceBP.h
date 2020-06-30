#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/geometry_msgs/ROSMsgPoseBP.h"
#include "ROSMsgPoseWithCovarianceBP.generated.h"

/**
 *  Low Precision variant of the UROSMsgPoseWithCovariance, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPoseWithCovarianceBP : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgPoseWithCovarianceBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/PoseWithCovariance";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseWithCovarianceBP* Create(UROSMsgPoseBP* Pose, const TArray<float>& Covariance);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseWithCovarianceBP* CreateEmpty();
	
	/* Data */
	/*
		Row-major representation of the 6x6 covariance matrix
		The orientation parameters use a fixed-axis representation.
		In order, the parameters are:
		(x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgPoseBP* Pose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Covariance;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
