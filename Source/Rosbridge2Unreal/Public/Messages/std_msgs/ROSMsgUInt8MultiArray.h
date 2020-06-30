#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgMultiArrayLayout.h"
#include "ROSMsgUInt8MultiArray.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgUInt8MultiArray : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgUInt8MultiArray(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/UInt8MultiArray";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgUInt8MultiArray* Create(TArray<uint8> Data, UROSMsgMultiArrayLayout* Layout);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgMultiArrayLayout* Layout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<uint8> Data;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
