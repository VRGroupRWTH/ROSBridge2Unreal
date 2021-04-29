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
	DataHelpers::Append<FString>(OutMessage, "label", Label);
	DataHelpers::Append<uint32>(OutMessage, "size", Size);
	DataHelpers::Append<uint32>(OutMessage, "stride", Stride);
}

bool UROSMsgMultiArrayDimension::FromData(const ROSData& Message)
{
	return DataHelpers::Extract<FString>(Message, "label", Label)
	&& DataHelpers::Extract<int64>(Message, "size", Size)
	&& DataHelpers::Extract<int64>(Message, "stride", Stride);
}

