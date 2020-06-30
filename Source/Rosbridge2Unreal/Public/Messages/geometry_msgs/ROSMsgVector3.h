#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgVector3.generated.h"

/**
 * Since 'double' is not supported in Blueprints, refer to the BP class of this one, if you want to use it in Blueprints
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSMsgVector3 : public UROSMessageBase
{
	GENERATED_BODY()
		
public:
	/* Construction */
	UROSMsgVector3(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Vector3";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgVector3* CreateFromVector(const FVector& Data);
	static UROSMsgVector3* Create(const double& X, const double& Y, const double& Z);

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
