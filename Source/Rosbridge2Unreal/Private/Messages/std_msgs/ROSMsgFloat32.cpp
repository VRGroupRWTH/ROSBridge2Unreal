#include "Messages/std_msgs/ROSMsgFloat32.h"
#include "DataHelpers.h"

UROSMsgFloat32* UROSMsgFloat32::Create(const float Data)
{
	UROSMsgFloat32* Message = NewObject<UROSMsgFloat32>();
	Message->Data = Data;
	return Message;
}

void UROSMsgFloat32::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<float>(OutMessage, "data", Data);
}

bool UROSMsgFloat32::FromData(const ROSData& Message)
{
	return DataHelpers::Extract<float>(Message, "data", Data);
}