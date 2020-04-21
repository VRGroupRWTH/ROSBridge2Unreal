// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSServiceUnadvertiseMessage.h"
#include "DataHelpers.h"

void UROSServiceUnadvertiseMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "unadvertise_service");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "service", ServiceName);
}

bool UROSServiceUnadvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "service", ServiceName);
}