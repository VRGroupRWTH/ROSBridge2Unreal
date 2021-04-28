#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgQuaternion.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgQuaternion : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgQuaternion(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Quaternion";}
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgQuaternion* Create(const FQuat& Data);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgQuaternion* CreateEmpty();

	/* Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FQuat AsFQuat() const;
	UFUNCTION(BlueprintCallable) void SetFromFQuat(const FQuat& InQuat);
	
	/* Data */
	double X;
	double Y;
	double Z;
	double W;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
