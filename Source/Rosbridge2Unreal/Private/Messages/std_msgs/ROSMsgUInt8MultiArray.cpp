#include "Messages/std_msgs/ROSMsgUInt8MultiArray.h"
#include "DataHelpers.h"

UROSMsgUInt8MultiArray* UROSMsgUInt8MultiArray::Create(const TArray<uint8>& Data, UROSMsgMultiArrayLayout* Layout)
{
	UROSMsgUInt8MultiArray* Message = NewObject<UROSMsgUInt8MultiArray>();
	Message->Data = Data;
	Message->Layout = Layout;
	return Message;
}

void UROSMsgUInt8MultiArray::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<TArray<uint8>>(OutMessage, "data", Data);
	DataHelpers::Append<UROSMsgMultiArrayLayout*>(OutMessage, "layout", Layout);
}

bool UROSMsgUInt8MultiArray::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<TArray<uint8>>(Message, "data", Data) &&
		DataHelpers::Extract<UROSMsgMultiArrayLayout*>(Message, "layout", Layout);
}