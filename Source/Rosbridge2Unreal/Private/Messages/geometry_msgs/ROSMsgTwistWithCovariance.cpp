#include "Messages/geometry_msgs/ROSMsgTwistWithCovariance.h"
#include "DataHelpers.h"
#include "LogCategory.h"

UROSMsgTwistWithCovariance* UROSMsgTwistWithCovariance::Create(UROSMsgTwist* Twist, const TArray<double>& Covariance)
{
	UROSMsgTwistWithCovariance* Message = NewObject<UROSMsgTwistWithCovariance>();
	Message->Twist = Twist;
	Message->Covariance = Covariance;
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgTwistWithCovariance does not have 36 values, it has %d"), Covariance.Num());
	return Message;
}

UROSMsgTwistWithCovariance* UROSMsgTwistWithCovariance::Create(UROSMsgTwist* Twist, const TArray<float>& Covariance)
{
	UROSMsgTwistWithCovariance* Message = NewObject<UROSMsgTwistWithCovariance>();
	Message->Twist = Twist;
	Message->SetCovarianceFromFloatArray(Covariance);
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgTwistWithCovariance does not have 36 values, it has %d"), Covariance.Num());
	return Message;
}

UROSMsgTwistWithCovariance* UROSMsgTwistWithCovariance::CreateEmpty()
{
	UROSMsgTwistWithCovariance* Message = NewObject<UROSMsgTwistWithCovariance>();
	Message->Twist = NewObject<UROSMsgTwist>(Message);
	Message->Covariance = TArray<double>();
	Message->Covariance.SetNumZeroed(36);
	return Message;
}

TArray<float> UROSMsgTwistWithCovariance::CovarianceAsFloatArray() const
{
	TArray<float> Result;
	Result.Append(Covariance);
	return Result;
}

void UROSMsgTwistWithCovariance::SetCovarianceFromFloatArray(const TArray<float>& InArray)
{
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgPoseWithCovariance does not have 36 values, it has %d"), Covariance.Num());

	Covariance.Empty();
	Covariance.Append(InArray);
}

void UROSMsgTwistWithCovariance::ToData(ROSData& OutMessage) const
{
	if (Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Covariance Matrix in UROSMsgTwistWithCovariance does not have 36 values, it has %d"), Covariance.Num());

	DataHelpers::Append<UROSMsgTwist*>(OutMessage, "twist", Twist);
	DataHelpers::Append<TArray<double>>(OutMessage, "covariance", Covariance);
}

bool UROSMsgTwistWithCovariance::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<UROSMsgTwist*>(Message, "twist", Twist) &&
		DataHelpers::Extract<TArray<double>>(Message, "covariance", Covariance) &&
		Covariance.Num() == 36;
}
