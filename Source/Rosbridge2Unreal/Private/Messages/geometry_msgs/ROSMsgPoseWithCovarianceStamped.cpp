#include "Messages/geometry_msgs/ROSMsgPoseWithCovarianceStamped.h"
#include "DataHelpers.h"

FVector UROSMsgPoseWithCovarianceStamped::GetPositionInUnrealCoordinateFrame() const
{
	if(!Pose->IsValidLowLevelFast() || !Pose->Pose->IsValidLowLevelFast()) return FVector();
	const FVector Position = Pose->Pose->PositionAsFVector();
	return FVector(Position.Y, Position.X, Position.Z) * FVector(100,100,100);
}

FQuat UROSMsgPoseWithCovarianceStamped::GetRotationInUnrealCoordinateFrame() const
{
	if(!Pose->IsValidLowLevelFast() || !Pose->Pose->IsValidLowLevelFast()) return FQuat();
	const FQuat Rotation = Pose->Pose->OrientationAsQuad();
	return FQuat(Rotation.Y, -Rotation.X, Rotation.Z, -Rotation.W);
}

void UROSMsgPoseWithCovarianceStamped::ToData(ROSData& Message) const
{
	ROSData SubElementHeader;
	ROSData SubElementPose;
	Header->ToData(SubElementHeader);
	Pose->ToData(SubElementPose);
	DataHelpers::AppendSubDocument(Message,  "header", SubElementHeader);
	DataHelpers::AppendSubDocument(Message,  "pose", SubElementPose);
}

bool UROSMsgPoseWithCovarianceStamped::FromData(const ROSData& Message)
{
	if(!Header) Header = NewObject<UROSMsgHeader>(this);
	if(!Pose) Pose = NewObject<UROSMsgPoseWithCovariance>(this);
	
	ROSData SubElementHeader;
	ROSData SubElementPose;
	
	return DataHelpers::ExtractSubDocument(Message,  "header", SubElementHeader) && Header->FromData(SubElementHeader)
	&& DataHelpers::ExtractSubDocument(Message,  "pose", SubElementPose) && Pose->FromData(SubElementPose);
}
