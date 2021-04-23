#pragma once

#include "CoreMinimal.h"
#include "Messages/geometry_msgs/ROSMsgPoseWithCovariance.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "ROSMsgPoseWithCovarianceStamped.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPoseWithCovarianceStamped : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgPoseWithCovarianceStamped(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/PoseWithCovarianceStamped";}

	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseWithCovarianceStamped* Create(UROSMsgHeader* Header, UROSMsgPoseWithCovariance* Pose);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseWithCovarianceStamped* CreateEmpty();
	
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector GetPositionInUnrealCoordinateFrame() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) FQuat GetRotationInUnrealCoordinateFrame() const;
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgPoseWithCovariance* Pose;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
