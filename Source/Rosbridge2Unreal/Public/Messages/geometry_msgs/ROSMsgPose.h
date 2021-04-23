#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "ROSMsgPose.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgPose : public UROSMessageBase
{
	GENERATED_BODY()
	
public:
	/* Construction */
	UROSMsgPose(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override {return "geometry_msgs/Pose";};
	UFUNCTION(BlueprintCallable)  static UROSMsgPose* CreateFromPositionOrientation(const FVector& Position, const FQuat& Orientation);
	static UROSMsgPose* Create(const double& Px = 0.0, const double& Py = 0.0, const double& Pz = 0.0, const double& Qx = 0.0, const double& Qy = 0.0, const double& Qz = 0.0, const double& Qw = 0.0);

	/* Blueprint functions. Ease of use. Lowers the precision */
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector PositionAsFVector() const;
	UFUNCTION(BlueprintCallable) void SetPositionFromFVector(const FVector InPosition);
	UFUNCTION(BlueprintCallable, BlueprintPure) FQuat OrientationAsQuad() const;
	UFUNCTION(BlueprintCallable) void SetOrientationFromQuad(const FQuat InOrientation);
	
	/* Data */
	/* Position */
	double Px;
	double Py;
	double Pz;
	/* Orientation */
	double Qx;
	double Qy;
	double Qz;
	double Qw;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
