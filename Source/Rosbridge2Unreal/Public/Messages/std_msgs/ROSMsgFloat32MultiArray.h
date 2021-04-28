#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgMultiArrayLayout.h"
#include "ROSMsgFloat32MultiArray.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgFloat32MultiArray : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgFloat32MultiArray(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/Float32MultiArray";}
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgFloat32MultiArray* Create(const TArray<float>& Data, UROSMsgMultiArrayLayout* Layout);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgFloat32MultiArray* CreateWithEmptyLayout(const TArray<float>& Data);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgMultiArrayLayout* Layout = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Data;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
