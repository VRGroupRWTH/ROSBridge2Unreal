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
	DataHelpers::AppendTArray<uint8>(OutMessage, "data", Data, [](ROSData& Array, const char* Key, const uint8& TArrayValue)
	{
		DataHelpers::AppendUInt8(Array, Key, TArrayValue);
	});

	ROSData SubElement;
	Layout->ToData(SubElement);
	DataHelpers::AppendSubDocument(OutMessage,"layout", SubElement);
}

bool UROSMsgUInt8MultiArray::FromData(const ROSData& Message)
{
	if(!Layout) Layout = NewObject<UROSMsgMultiArrayLayout>(this);

	ROSData SubElement;
	
	return DataHelpers::ExtractTArray<uint8>(Message, "data", Data, [](const ROSData& Array, const char* Key, uint8& Result)
	{
		return DataHelpers::ExtractUInt8(Array, Key, Result);
	})
	&& DataHelpers::ExtractSubDocument(Message, "layout", SubElement) && Layout->FromData(SubElement);
}