#pragma once

#include "CoreMinimal.h"
#include "Messages/geometry_msgs/ROSMsgPoseWithCovarianceBP.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "ROSMsgPoseWithCovarianceStampedBP.generated.h"

/*
 * Low Precision variant of the UROSMsgPoseWithCovarianceStamped, that is blueprint compatible
*/
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPoseWithCovarianceStampedBP : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgPoseWithCovarianceStampedBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/PoseWithCovarianceStamped";}

	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseWithCovarianceStampedBP* Create(UROSMsgHeader* Header, UROSMsgPoseWithCovarianceBP* Pose);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseWithCovarianceStampedBP* CreateEmpty();

	UFUNCTION(BlueprintCallable, BlueprintPure) FVector GetPositionInUnrealCoordinateFrame() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) FQuat GetRotationInUnrealCoordinateFrame() const;
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgPoseWithCovarianceBP* Pose;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
