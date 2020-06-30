#include "Messages/geometry_msgs/ROSMsgPoseWithCovarianceBP.h"
#include "DataHelpers.h"
#include "LogCategory.h"

UROSMsgPoseWithCovarianceBP* UROSMsgPoseWithCovarianceBP::Create(UROSMsgPoseBP* Pose, const TArray<float>& Covariance)
{
	UROSMsgPoseWithCovarianceBP* Message = NewObject<UROSMsgPoseWithCovarianceBP>();
	Message->Pose = Pose;
	Message->Covariance = Covariance;
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgPoseWithCovariance does not have 36 values, it has %d"), Covariance.Num());
	return Message;
}

UROSMsgPoseWithCovarianceBP* UROSMsgPoseWithCovarianceBP::CreateEmpty()
{
	UROSMsgPoseWithCovarianceBP* Message = NewObject<UROSMsgPoseWithCovarianceBP>();
	Message->Pose = NewObject<UROSMsgPoseBP>(Message);
	Message->Covariance = TArray<float>();
	Message->Covariance.SetNumZeroed(36);
	return Message;
}

void UROSMsgPoseWithCovarianceBP::ToData(ROSData& Message) const
{
	ROSData SubElementPose;
	Pose->ToData(SubElementPose);
	DataHelpers::AppendSubDocument(Message,  "pose", SubElementPose);
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Covariance Matrix in UROSMsgPoseWithCovariance does not have 36 values, it has %d"), Covariance.Num());

	DataHelpers::AppendTArray<float>(Message, "covariance", Covariance, [](ROSData& Array, const char* Key, const float& TArrayValue)
	{
		DataHelpers::AppendFloat(Array, Key, TArrayValue);
	});
}

bool UROSMsgPoseWithCovarianceBP::FromData(const ROSData& Message)
{
	if(!Pose) Pose = NewObject<UROSMsgPoseBP>(this);
	
	ROSData SubElementPose;
	return DataHelpers::ExtractSubDocument(Message, "pose", SubElementPose) && Pose->FromData(SubElementPose)
	&& DataHelpers::ExtractTArray<float>(Message, "covariance", Covariance, [](const ROSData& Array, const char* Key, float& Result)
	{
		return DataHelpers::ExtractFloat(Array, Key, Result);
	});
}
