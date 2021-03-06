#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgFloat32.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgFloat32 : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgFloat32(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/Float32";}
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgFloat32* Create(const float Data);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Data;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
