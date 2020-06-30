#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgPoseBP.generated.h"

/**
 *  Low Precision variant of the UROSMsgPose, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPoseBP : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgPoseBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Pose";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseBP* CreateFromPositionOrientation(const FVector& Position, const FQuat& Orientation);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgPoseBP* Create(const float& Px = 0.0, const float& Py = 0.0, const float& Pz = 0.0, const float& Qx = 0.0, const float& Qy = 0.0, const float& Qz = 0.0, const float& Qw = 0.0);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Position;						
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuat Orientation;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
