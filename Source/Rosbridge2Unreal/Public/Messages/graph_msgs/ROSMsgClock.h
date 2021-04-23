#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgClock.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgClock : public UROSMessageBase
{
	GENERATED_BODY()

public:
	FString GetMessageType() override {return "rosgraph_msgs/Clock";};
	
	/* Convenience Methods */
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgClock* Create(int32 InSeconds, int32 InNanoSeconds);
	
	/* Request Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Seconds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 NanoSeconds;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
