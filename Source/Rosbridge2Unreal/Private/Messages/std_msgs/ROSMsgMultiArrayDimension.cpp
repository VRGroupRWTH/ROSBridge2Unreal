// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgMultiArrayDimension.h"
#include "DataHelpers.h"

UROSMsgMultiArrayDimension* UROSMsgMultiArrayDimension::Create(const FString Label, const int64 Size, const int64 Stride)
{
	UROSMsgMultiArrayDimension* Message = NewObject<UROSMsgMultiArrayDimension>();
	Message->Label = Label;
	Message->Size = Size;
	Message->Stride = Stride;
	return Message;
}


void UROSMsgMultiArrayDimension::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "label", Label);
	DataHelpers::AppendUInt32(Message, "size", Size);
	DataHelpers::AppendUInt32(Message, "stride", Stride);
}

bool UROSMsgMultiArrayDimension::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractString(Message, "label", Label)
	&& DataHelpers::ExtractInt64(Message, "size", Size)
	&& DataHelpers::ExtractInt64(Message, "stride", Stride);
}

