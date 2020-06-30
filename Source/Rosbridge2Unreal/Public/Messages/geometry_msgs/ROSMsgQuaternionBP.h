#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgQuaternionBP.generated.h"

/**
 *  Low Precision variant of the UROSMsgQuaternion, that is blueprint compatible
 */
UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgQuaternionBP : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgQuaternionBP(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Quaternion";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgQuaternionBP* Create(const FQuat& Data);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgQuaternionBP* CreateEmpty();
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuat Quaternion;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
