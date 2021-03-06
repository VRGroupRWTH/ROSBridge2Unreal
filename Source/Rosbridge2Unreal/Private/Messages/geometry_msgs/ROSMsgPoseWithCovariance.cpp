#include "Messages/geometry_msgs/ROSMsgPoseWithCovariance.h"
#include "DataHelpers.h"
#include "LogCategory.h"

UROSMsgPoseWithCovariance* UROSMsgPoseWithCovariance::Create(UROSMsgPose* Pose, const TArray<double>& Covariance)
{
	UROSMsgPoseWithCovariance* Message = NewObject<UROSMsgPoseWithCovariance>();
	Message->Pose = Pose;
	Message->Covariance = Covariance;
	if (Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgPoseWithCovariance does not have 36 values, it has %d"), Covariance.Num());
	return Message;
}

UROSMsgPoseWithCovariance* UROSMsgPoseWithCovariance::Create(UROSMsgPose* Pose, const TArray<float>& Covariance)
{
	UROSMsgPoseWithCovariance* Message = NewObject<UROSMsgPoseWithCovariance>();
	Message->Pose = Pose;
	Message->SetCovarianceFromFloatArray(Covariance);
	if (Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgPoseWithCovariance does not have 36 values, it has %d"), Covariance.Num());
	return Message;
}

UROSMsgPoseWithCovariance* UROSMsgPoseWithCovariance::CreateEmpty()
{
	UROSMsgPoseWithCovariance* Message = NewObject<UROSMsgPoseWithCovariance>();
	Message->Pose = NewObject<UROSMsgPose>(Message);
	Message->Covariance = TArray<double>();
	Message->Covariance.SetNumZeroed(36);
	return Message;
}

TArray<float> UROSMsgPoseWithCovariance::CovarianceAsFloatArray() const
{
	TArray<float> Result;
	Result.Append(Covariance);
	return Result;
}

void UROSMsgPoseWithCovariance::SetCovarianceFromFloatArray(const TArray<float>& InArray)
{
	if (InArray.Num() != 36)
	{
		UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgPoseWithCovariance does not have 36 values, it has %d"), Covariance.Num());
	}

	Covariance.Empty();
	Covariance.Append(InArray);
}

void UROSMsgPoseWithCovariance::ToData(ROSData& OutMessage) const
{
	if (Covariance.Num() != 36)
	{
		UE_LOG(LogROSBridge, Warning, TEXT("Covariance Matrix in UROSMsgPoseWithCovariance does not have 36 values, it has %d"), Covariance.Num());
	}

	DataHelpers::Append<UROSMsgPose*>(OutMessage, "pose", Pose);
	DataHelpers::Append<TArray<double>>(OutMessage, "covariance", Covariance);
}

bool UROSMsgPoseWithCovariance::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<UROSMsgPose*>(Message, "pose", Pose) &&
		DataHelpers::Extract<TArray<double>>(Message, "covariance", Covariance) &&
		Covariance.Num() == 36;
}
