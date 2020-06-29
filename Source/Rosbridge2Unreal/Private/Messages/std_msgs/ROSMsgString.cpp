// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/std_msgs/ROSMsgString.h"
#include "DataHelpers.h"

UROSMsgString* UROSMsgString::Create(FString Data)
{
	UROSMsgString* Message = NewObject<UROSMsgString>();
	Message->Data = Data;
	return Message;
}

void UROSMsgString::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "data", Data);
}

bool UROSMsgString::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractString(Message, "data", Data);
}