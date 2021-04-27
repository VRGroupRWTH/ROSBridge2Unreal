#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgString.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgString : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgString(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "std_msgs/String";}
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgString* Create(const FString& Data);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Data;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
