#include "Messages/geometry_msgs/ROSMsgPoseWithCovarianceStamped.h"
#include "DataHelpers.h"

UROSMsgPoseWithCovarianceStamped* UROSMsgPoseWithCovarianceStamped::Create(UROSMsgHeader* Header, UROSMsgPoseWithCovariance* Pose)
{
	UROSMsgPoseWithCovarianceStamped* Message = NewObject<UROSMsgPoseWithCovarianceStamped>();
	Message->Header = Header;
	Message->Pose = Pose;
	return Message;
}

UROSMsgPoseWithCovarianceStamped* UROSMsgPoseWithCovarianceStamped::CreateEmpty()
{
	UROSMsgPoseWithCovarianceStamped* Message = NewObject<UROSMsgPoseWithCovarianceStamped>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Pose = NewObject<UROSMsgPoseWithCovariance>(Message);
	return Message;
}

FVector UROSMsgPoseWithCovarianceStamped::GetPositionInUnrealCoordinateFrame() const
{
	if(!Pose->IsValidLowLevelFast() || !Pose->Pose->IsValidLowLevelFast()) return FVector();
	const FVector& Position = Pose->Pose->PositionAsFVector();
	return FVector(Position.Y, Position.X, Position.Z) * FVector(100,100,100);
}

FQuat UROSMsgPoseWithCovarianceStamped::GetRotationInUnrealCoordinateFrame() const
{
	if(!Pose->IsValidLowLevelFast() || !Pose->Pose->IsValidLowLevelFast()) return FQuat();
	const FQuat& Rotation = Pose->Pose->OrientationAsQuad();
	return FQuat(Rotation.Y, -Rotation.X, Rotation.Z, -Rotation.W);
}

void UROSMsgPoseWithCovarianceStamped::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<UROSMsgHeader*>(OutMessage, "header", Header);
	DataHelpers::Append<UROSMsgPoseWithCovariance*>(OutMessage, "pose", Pose);
}

bool UROSMsgPoseWithCovarianceStamped::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<UROSMsgHeader*>(Message, "header", Header) &&
		DataHelpers::Extract<UROSMsgPoseWithCovariance*>(Message, "pose", Pose);
}
