#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "Messages/geometry_msgs/ROSMsgTransform.h"
#include "ROSMsgTransformStamped.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgTransformStamped : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTransformStamped(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/TransformStamped";}
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformStamped* Create(UROSMsgHeader* Header, const FString& ChildFrameID, UROSMsgTransform* Transform);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTransformStamped* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgTransform* Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ChildFrameID;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
