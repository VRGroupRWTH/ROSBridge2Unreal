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
	DataHelpers::Append<FString>(OutMessage, "data", Data);
}

bool UROSMsgString::FromData(const ROSData& Message)
{
	return DataHelpers::Extract<FString>(Message, "data", Data);
}