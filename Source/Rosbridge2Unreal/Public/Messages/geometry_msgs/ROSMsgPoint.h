#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgPoint.generated.h"

/**
 * Since 'double' is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgPoint : public UROSMessageBase
{
	GENERATED_BODY()
		
public:
	/* Construction */
	UROSMsgPoint(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Point";};
	static UROSMsgPoint* CreateFromVector(const FVector& Data);
	static UROSMsgPoint* Create(const double& X, const double& Y, const double& Z);

	/* Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector AsFVector() const;
	
	/* Data */
	double X;
	double Y;
	double Z;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
