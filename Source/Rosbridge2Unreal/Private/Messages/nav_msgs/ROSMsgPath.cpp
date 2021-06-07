#include "Messages/nav_msgs/ROSMsgPath.h"
#include "DataHelpers.h"

UROSMsgPath* UROSMsgPath::CreateEmpty()
{
	UROSMsgPath* Message = NewObject<UROSMsgPath>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Poses = TArray<UROSMsgPoseStamped*>();
	return Message;
}

void UROSMsgPath::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<UROSMsgHeader*>(OutMessage, "header", Header);
	DataHelpers::Append<TArray<UROSMsgPoseStamped*>>(OutMessage, "poses", Poses);
}

bool UROSMsgPath::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<UROSMsgHeader*>(Message, "header", Header) &&
		DataHelpers::Extract<TArray<UROSMsgPoseStamped*>>(Message, "poses", Poses);
}
