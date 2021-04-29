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
	DataHelpers::Append<UROSMsgHeader*>(OutMessage, "header", Header);
	DataHelpers::Append<FString>(OutMessage, "child_frame_id", ChildFrameID);
	DataHelpers::Append<UROSMsgTransform*>(OutMessage, "transform", Transform);
}

bool UROSMsgTransformStamped::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<UROSMsgHeader*>(Message, "header", Header) &&
		DataHelpers::Extract<FString>(Message, "child_frame_id", ChildFrameID) &&
		DataHelpers::Extract<UROSMsgTransform*>(Message, "transform", Transform);
}
