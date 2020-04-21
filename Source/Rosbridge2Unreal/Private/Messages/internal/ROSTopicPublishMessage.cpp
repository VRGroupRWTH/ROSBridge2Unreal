// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSTopicPublishMessage.h"
#include "DataHelpers.h"

void UROSTopicPublishMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "publish");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "topic", TopicName);
	DataHelpers::AppendSubDocument(Message, "msg", Data);
}

bool UROSTopicPublishMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName)
	&& DataHelpers::ExtractSubDocument(Message, "msg", Data);
}