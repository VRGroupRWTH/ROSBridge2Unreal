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
	DataHelpers::Append<TArray<UROSMsgMultiArrayDimension*>>(OutMessage, "dim", Dimensions);
	DataHelpers::Append<uint32>(OutMessage, "data_offset", DataOffset);
}

bool UROSMsgMultiArrayLayout::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<TArray<UROSMsgMultiArrayDimension*>>(Message, "dim", Dimensions) &&
		DataHelpers::Extract<int64>(Message, "data_offset", DataOffset);
}