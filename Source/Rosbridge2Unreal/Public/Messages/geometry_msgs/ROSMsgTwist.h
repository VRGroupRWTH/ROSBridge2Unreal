#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgTwist.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgTwist : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgTwist(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Twist";}
	static UROSMsgTwist* Create(double Lx, double Ly, double Lz, double Ax, double Ay, double Az);
	UFUNCTION(BlueprintCallable) static UROSMsgTwist* CreateFromVectors(const FVector& InLinear, const FVector& InAngular);
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTwist* CreateEmpty();

	/* Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector LinearAsFVector() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector AngularAsFVector() const;
	UFUNCTION(BlueprintCallable) void SetLinearFromFVector(const FVector InVector);
	UFUNCTION(BlueprintCallable) void SetAngularFromFVector(const FVector InVector);
	
	/* Data */
	double LinearX;
	double LinearY;
	double LinearZ;
	double AngularX;
	double AngularY;
	double AngularZ;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
