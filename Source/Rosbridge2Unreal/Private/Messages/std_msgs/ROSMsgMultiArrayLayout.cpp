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
		ROSData SubElement;
		TArrayValue->ToData(SubElement);
		DataHelpers::AppendSubDocument(Array,  Key, SubElement);
	});
	DataHelpers::AppendUInt32(OutMessage, "data_offset", DataOffset);
}

bool UROSMsgMultiArrayLayout::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractTArrayOfUObjects<UROSMsgMultiArrayDimension>(Message, "dim", Dimensions, this, [](const ROSData& Array, const char* Key, UROSMsgMultiArrayDimension*& Result)
	{
		ROSData SubElement;
		return DataHelpers::ExtractSubDocument(Array, Key, SubElement) && Result->FromData(SubElement);
	})
	&& DataHelpers::ExtractInt64(Message, "data_offset", DataOffset);
}