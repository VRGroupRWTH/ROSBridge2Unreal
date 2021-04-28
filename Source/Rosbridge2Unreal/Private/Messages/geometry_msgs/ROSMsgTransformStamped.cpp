#include "Messages/geometry_msgs/ROSMsgTransformStamped.h"
#include "DataHelpers.h"

UROSMsgTransformStamped* UROSMsgTransformStamped::Create(UROSMsgHeader* Header, const FString& ChildFrameID, UROSMsgTransform* Transform)
{
	UROSMsgTransformStamped* Message = NewObject<UROSMsgTransformStamped>();
	Message->Header = Header;
	Message->ChildFrameID = ChildFrameID;
	Message->Transform = Transform;
	return Message;
}

UROSMsgTransformStamped* UROSMsgTransformStamped::CreateEmpty()
{
	UROSMsgTransformStamped* Message = NewObject<UROSMsgTransformStamped>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Transform = NewObject<UROSMsgTransform>(Message);
	Message->ChildFrameID = "";
	return Message;
}

void UROSMsgTransformStamped::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendSubMessage(OutMessage, "header", Header);
	DataHelpers::AppendString(OutMessage, "child_frame_id", ChildFrameID);
	DataHelpers::AppendSubMessage(OutMessage, "transform", Transform);
}

bool UROSMsgTransformStamped::FromData(const ROSData& Message)
{
	return
		DataHelpers::ExtractSubMessage(Message, "header", Header) &&
		DataHelpers::ExtractString(Message, "child_frame_id", ChildFrameID) &&
		DataHelpers::ExtractSubMessage(Message, "transform", Transform);
}
