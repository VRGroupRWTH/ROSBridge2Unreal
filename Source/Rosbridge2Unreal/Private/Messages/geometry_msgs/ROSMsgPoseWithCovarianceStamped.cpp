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
