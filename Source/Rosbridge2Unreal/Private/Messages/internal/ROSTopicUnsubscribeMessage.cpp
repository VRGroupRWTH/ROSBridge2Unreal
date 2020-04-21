// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSTopicUnsubscribeMessage.h"
#include "DataHelpers.h"

void UROSTopicUnsubscribeMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "unsubscribe");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "topic", TopicName);
}

bool UROSTopicUnsubscribeMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName);
}