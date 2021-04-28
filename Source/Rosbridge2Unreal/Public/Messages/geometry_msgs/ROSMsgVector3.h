#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgVector3.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgVector3 : public UROSMessageBase
{
	GENERATED_BODY()
		
public:
	/* Construction */
	UROSMsgVector3(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Vector3";}
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgVector3* CreateFromVector(const FVector& Data);
	static UROSMsgVector3* Create(double X, double Y, double Z);

	/* Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector AsFVector() const;
	UFUNCTION(BlueprintCallable) void SetFromFVector(const FVector& InVector);
	
	/* Data */
	double X;
	double Y;
	double Z;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
