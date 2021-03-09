// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/rosapi/ROSSrvTopics.h"
#include "DataHelpers.h"

UROSSrvTopics* UROSSrvTopics::CreateEmpty()
{
	return NewObject<UROSSrvTopics>();
}

void UROSSrvTopics::RequestToData(ROSData& Message) const {}

bool UROSSrvTopics::RequestFromData(const ROSData& Message) {return true;}

void UROSSrvTopics::ResponseToData(ROSData& Message) const
{
	DataHelpers::AppendTArray<FString>(Message, "topics", Topics, [](ROSData& Message, const char* Key, const FString& Value)
	{
		DataHelpers::AppendString(Message, Key, Value);
	});
}

bool UROSSrvTopics::ResponseFromData(const ROSData& Message)
{
	return DataHelpers::ExtractTArray<FString>(Message, "topics", Topics, [](const ROSData& Message, const char* Key, FString& Value)
	{
		return DataHelpers::ExtractString(Message, Key, Value);
	});
}
