#include "Messages/std_msgs/ROSMsgMultiArrayLayout.h"
#include "DataHelpers.h"

UROSMsgMultiArrayLayout* UROSMsgMultiArrayLayout::CreateEmpty()
{
	return NewObject<UROSMsgMultiArrayLayout>();
}

UROSMsgMultiArrayLayout* UROSMsgMultiArrayLayout::Create(const TArray<UROSMsgMultiArrayDimension*>& InDimensions, int64 InDataOffset)
{
	UROSMsgMultiArrayLayout* Message = NewObject<UROSMsgMultiArrayLayout>();
	Message->Dimensions = InDimensions;
	Message->DataOffset = InDataOffset;
	return Message;
}

void UROSMsgMultiArrayLayout::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendTArray<UROSMsgMultiArrayDimension*>(OutMessage, "dim", Dimensions, [](ROSData& Array, const char* Key, UROSMsgMultiArrayDimension* const& TArrayValue)
	{
		DataHelpers::AppendSubMessage(Array, Key, TArrayValue);
	});
	DataHelpers::AppendUInt32(OutMessage, "data_offset", DataOffset);
}

bool UROSMsgMultiArrayLayout::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractTArrayOfUObjects<UROSMsgMultiArrayDimension>(Message, "dim", Dimensions, this, [](const ROSData& Array, const char* Key, UROSMsgMultiArrayDimension*& Result)
	{
		return DataHelpers::ExtractSubMessage(Array, Key, Result);
	})
	&& DataHelpers::ExtractInt64(Message, "data_offset", DataOffset);
}