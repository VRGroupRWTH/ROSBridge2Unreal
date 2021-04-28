#include "Messages/geometry_msgs/ROSMsgPoseStamped.h"
#include "DataHelpers.h"

UROSMsgPoseStamped* UROSMsgPoseStamped::Create(UROSMsgHeader* Header, UROSMsgPose* Pose)
{
	UROSMsgPoseStamped* Message = NewObject<UROSMsgPoseStamped>();
	Message->Header = Header;
	Message->Pose = Pose;
	return Message;
}

UROSMsgPoseStamped* UROSMsgPoseStamped::CreateEmpty()
{
	UROSMsgPoseStamped* Message = NewObject<UROSMsgPoseStamped>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Pose = NewObject<UROSMsgPose>(Message);
	return Message;
}

void UROSMsgPoseStamped::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendSubMessage(OutMessage, "header", Header);
	DataHelpers::AppendSubMessage(OutMessage, "pose", Pose);
}

bool UROSMsgPoseStamped::FromData(const ROSData& Message)
{
	return
		DataHelpers::ExtractSubMessage(Message, "header", Header) &&
		DataHelpers::ExtractSubMessage(Message, "pose", Pose);
}
