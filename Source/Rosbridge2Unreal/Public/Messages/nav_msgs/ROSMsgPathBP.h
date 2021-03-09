#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "Messages/geometry_msgs/ROSMsgPoseStampedBP.h"
#include "ROSMsgPathBP.generated.h"

/*
 * Low Precision variant of the UROSMsgPoseWithCovariance, that is blueprint compatible
*/
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPathBP : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgPathBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "nav_msgs/Path";}
	UFUNCTION(BlueprintCallable) static UROSMsgPathBP* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UROSMsgPoseStampedBP*> Poses;
	UFUNCTION(BlueprintCallable) void AddPoseInUnrealCoordinateFrame(const FTransform& Transform, UROSMsgHeader* HeaderIn);

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
