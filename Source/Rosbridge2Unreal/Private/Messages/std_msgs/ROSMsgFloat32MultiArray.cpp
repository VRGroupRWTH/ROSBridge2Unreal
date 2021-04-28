#include "Messages/std_msgs/ROSMsgFloat32MultiArray.h"
#include "DataHelpers.h"

UROSMsgFloat32MultiArray* UROSMsgFloat32MultiArray::Create(const TArray<float>& Data, UROSMsgMultiArrayLayout* Layout)
{
	UROSMsgFloat32MultiArray* Message = NewObject<UROSMsgFloat32MultiArray>();
	Message->Data = Data;
	Message->Layout = Layout;
	return Message;
}

UROSMsgFloat32MultiArray* UROSMsgFloat32MultiArray::CreateWithEmptyLayout(const TArray<float>& Data)
{
	UROSMsgFloat32MultiArray* Message = NewObject<UROSMsgFloat32MultiArray>();
	Message->Data = Data;
	Message->Layout = NewObject<UROSMsgMultiArrayLayout>(Message);
	return Message;
}

void UROSMsgFloat32MultiArray::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendTArray<float>(OutMessage, "data", Data, [](ROSData& Array, const char* Key, const float& TArrayValue)
	{
		DataHelpers::AppendFloat(Array, Key, TArrayValue);
	});
	DataHelpers::AppendSubMessage(OutMessage,"layout", Layout);
}

bool UROSMsgFloat32MultiArray::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractTArray<float>(Message, "data", Data, [](const ROSData& Array, const char* Key, float& Result)
	{
		return DataHelpers::ExtractFloat(Array, Key, Result);
	})
	&& DataHelpers::ExtractSubMessage(Message, "layout", Layout);
}