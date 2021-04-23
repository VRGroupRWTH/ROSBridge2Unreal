#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "Messages/geometry_msgs/ROSMsgPoseStamped.h"
#include "ROSMsgPath.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPath : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgPath(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "nav_msgs/Path";}
	UFUNCTION(BlueprintCallable) static UROSMsgPath* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UROSMsgPoseStamped*> Poses;
	UFUNCTION(BlueprintCallable) void AddPoseInUnrealCoordinateFrame(const FTransform& Transform, UROSMsgHeader* HeaderIn);

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
