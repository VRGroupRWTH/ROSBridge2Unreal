#include "Messages/std_msgs/ROSMsgString.h"
#include "DataHelpers.h"

UROSMsgString* UROSMsgString::Create(const FString& InData)
{
	UROSMsgString* Message = NewObject<UROSMsgString>();
	Message->Data = InData;
	return Message;
}

void UROSMsgString::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "data", Data);
}

bool UROSMsgString::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractString(Message, "data", Data);
}