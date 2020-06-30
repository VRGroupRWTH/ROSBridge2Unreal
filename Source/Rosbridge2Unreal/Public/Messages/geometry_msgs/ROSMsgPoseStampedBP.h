#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "Messages/geometry_msgs/ROSMsgPoseBP.h"
#include "ROSMsgPoseStampedBP.generated.h"

/**
 * Low Precision variant of the UROSMsgPose, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPoseStampedBP : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgPoseStampedBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/PoseStamped";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseStampedBP* Create(UROSMsgHeader* Header, UROSMsgPoseBP* Pose);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseStampedBP* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgPoseBP* Pose;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
