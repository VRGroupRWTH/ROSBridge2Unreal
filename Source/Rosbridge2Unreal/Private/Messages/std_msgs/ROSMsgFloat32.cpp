// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgFloat32.h"
#include "DataHelpers.h"

UROSMsgFloat32* UROSMsgFloat32::Create(const float Data)
{
	UROSMsgFloat32* Message = NewObject<UROSMsgFloat32>();
	Message->Data = Data;
	return Message;
}

void UROSMsgFloat32::ToData(ROSData& Message) const
{
	DataHelpers::AppendFloat(Message, "data", Data);
}

bool UROSMsgFloat32::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractFloat(Message, "data", Data);
}