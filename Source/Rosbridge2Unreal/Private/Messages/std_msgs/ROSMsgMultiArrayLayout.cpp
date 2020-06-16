// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgMultiArrayLayout.h"
#include "DataHelpers.h"

UROSMsgMultiArrayLayout* UROSMsgMultiArrayLayout::CreateEmpty()
{
	return NewObject<UROSMsgMultiArrayLayout>();
}

UROSMsgMultiArrayLayout* UROSMsgMultiArrayLayout::Create(TArray<UROSMsgMultiArrayDimension*>& Dimensions, const int64& DataOffset)
{
	UROSMsgMultiArrayLayout* Message = NewObject<UROSMsgMultiArrayLayout>();
	Message->Dimensions = Dimensions;
	Message->DataOffset = DataOffset;
	return Message;
}

void UROSMsgMultiArrayLayout::ToData(ROSData& Message) const
{
	DataHelpers::AppendTArray<UROSMsgMultiArrayDimension*>(Message, "dim", Dimensions, [](ROSData& Array, const char* Key, UROSMsgMultiArrayDimension* const& TArrayValue)
	{
		ROSData SubElement;
		TArrayValue->ToData(SubElement);
		DataHelpers::AppendSubDocument(Array,  Key, SubElement);
	});
	DataHelpers::AppendUInt32(Message, "data_offset", DataOffset);
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