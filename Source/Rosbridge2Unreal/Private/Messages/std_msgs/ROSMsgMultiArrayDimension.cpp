#include "Messages/std_msgs/ROSMsgMultiArrayDimension.h"
#include "DataHelpers.h"

UROSMsgMultiArrayDimension* UROSMsgMultiArrayDimension::Create(const FString& Label, const int64 Size, const int64 Stride)
{
	UROSMsgMultiArrayDimension* Message = NewObject<UROSMsgMultiArrayDimension>();
	Message->Label = Label;
	Message->Size = Size;
	Message->Stride = Stride;
	return Message;
}


void UROSMsgMultiArrayDimension::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "label", Label);
	DataHelpers::AppendUInt32(OutMessage, "size", Size);
	DataHelpers::AppendUInt32(OutMessage, "stride", Stride);
}

bool UROSMsgMultiArrayDimension::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractString(Message, "label", Label)
	&& DataHelpers::ExtractInt64(Message, "size", Size)
	&& DataHelpers::ExtractInt64(Message, "stride", Stride);
}

