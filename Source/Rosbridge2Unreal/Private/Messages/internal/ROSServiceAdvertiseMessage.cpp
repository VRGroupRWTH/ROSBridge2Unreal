// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/internal/ROSServiceAdvertiseMessage.h"
#include "DataHelpers.h"

void UROSServiceAdvertiseMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "advertise_service");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "service", ServiceName);
	DataHelpers::AppendString(Message, "type", ServiceType);
}

bool UROSServiceAdvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "service", ServiceName)
	&& DataHelpers::ExtractString(Message, "type", ServiceType);
}