#include "Messages/geometry_msgs/ROSMsgPoseWithCovarianceStampedBP.h"
#include "DataHelpers.h"

UROSMsgPoseWithCovarianceStampedBP* UROSMsgPoseWithCovarianceStampedBP::Create(UROSMsgHeader* Header, UROSMsgPoseWithCovarianceBP* Pose)
{
	UROSMsgPoseWithCovarianceStampedBP* Message = NewObject<UROSMsgPoseWithCovarianceStampedBP>();
	Message->Header = Header;
	Message->Pose = Pose;
	return Message;
}

UROSMsgPoseWithCovarianceStampedBP* UROSMsgPoseWithCovarianceStampedBP::CreateEmpty()
{
	UROSMsgPoseWithCovarianceStampedBP* Message = NewObject<UROSMsgPoseWithCovarianceStampedBP>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Pose = NewObject<UROSMsgPoseWithCovarianceBP>(Message);
	return Message;
}

FVector UROSMsgPoseWithCovarianceStampedBP::GetPositionInUnrealCoordinateFrame() const
{
	if(!Pose->IsValidLowLevelFast() || !Pose->Pose->IsValidLowLevelFast()) return FVector();
	const FVector Position = Pose->Pose->Position;
	return FVector(Position.Y, Position.X, Position.Z) * FVector(100,100,100);
}

FQuat UROSMsgPoseWithCovarianceStampedBP::GetRotationInUnrealCoordinateFrame() const
{
	if(!Pose->IsValidLowLevelFast() || !Pose->Pose->IsValidLowLevelFast()) return FQuat();
	const FQuat Rotation = Pose->Pose->Orientation;
	return FQuat(Rotation.Y, -Rotation.X, Rotation.Z, -Rotation.W);
}

void UROSMsgPoseWithCovarianceStampedBP::ToData(ROSData& Message) const
{
	ROSData SubElementHeader;
	ROSData SubElementPose;
	Header->ToData(SubElementHeader);
	Pose->ToData(SubElementPose);
	DataHelpers::AppendSubDocument(Message,  "header", SubElementHeader);
	DataHelpers::AppendSubDocument(Message,  "pose", SubElementPose);
}

bool UROSMsgPoseWithCovarianceStampedBP::FromData(const ROSData& Message)
{
	if(!Header) Header = NewObject<UROSMsgHeader>(this);
	if(!Pose) Pose = NewObject<UROSMsgPoseWithCovarianceBP>(this);
	
	ROSData SubElementHeader;
	ROSData SubElementPose;
	
	return DataHelpers::ExtractSubDocument(Message,  "header", SubElementHeader) && Header->FromData(SubElementHeader)
	&& DataHelpers::ExtractSubDocument(Message,  "pose", SubElementPose) && Pose->FromData(SubElementPose);
}
