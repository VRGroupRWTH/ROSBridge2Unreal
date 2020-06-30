#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgTwistBP.generated.h"

/**
 * Low Precision variant of the UROSMsgTwist, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgTwistBP : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTwistBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Twist";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwistBP* Create(const float& Lx,const float& Ly,const float& Lz,const float& Ax,const float& Ay,const float& Az);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwistBP* CreateFromVectors(const FVector& Linear, const FVector& Angular);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwistBP* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Linear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Angular;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
