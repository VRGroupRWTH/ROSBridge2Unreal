// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/std_msgs/ROSMsgFloat32MultiArray.h"
#include "DataHelpers.h"

UROSMsgFloat32MultiArray* UROSMsgFloat32MultiArray::Create(TArray<float>& Data, UROSMsgMultiArrayLayout* Layout)
{
	UROSMsgFloat32MultiArray* Message = NewObject<UROSMsgFloat32MultiArray>();
	Message->Data = Data;
	Message->Layout = Layout;
	return Message;
}

UROSMsgFloat32MultiArray* UROSMsgFloat32MultiArray::CreateWithEmptyLayout(TArray<float>& Data)
{
	UROSMsgFloat32MultiArray* Message = NewObject<UROSMsgFloat32MultiArray>();
	Message->Data = Data;
	Message->Layout = NewObject<UROSMsgMultiArrayLayout>(Message);
	return Message;
}

void UROSMsgFloat32MultiArray::ToData(ROSData& Message) const
{
	DataHelpers::AppendTArray<float>(Message, "data", Data, [](ROSData& Array, const char* Key, const float& TArrayValue)
	{
		DataHelpers::AppendFloat(Array, Key, TArrayValue);
	});

	ROSData SubElement;
	Layout->ToData(SubElement);
	DataHelpers::AppendSubDocument(Message,"layout", SubElement);
}

bool UROSMsgFloat32MultiArray::FromData(const ROSData& Message)
{
	if(!Layout) Layout = NewObject<UROSMsgMultiArrayLayout>(this);

	ROSData SubElement;
	
	return DataHelpers::ExtractTArray<float>(Message, "data", Data, [](const ROSData& Array, const char* Key, float& Result)
	{
		return DataHelpers::ExtractFloat(Array, Key, Result);
	})
	&& DataHelpers::ExtractSubDocument(Message, "layout", SubElement) && Layout->FromData(SubElement);
}