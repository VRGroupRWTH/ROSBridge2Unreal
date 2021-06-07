// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/rosapi/ROSSrvTopics.h"
#include "DataHelpers.h"

UROSSrvTopics* UROSSrvTopics::CreateEmpty()
{
	return NewObject<UROSSrvTopics>();
}

void UROSSrvTopics::RequestToData(ROSData& OutMessage) const {}

bool UROSSrvTopics::RequestFromData(const ROSData& Message) {return true;}

void UROSSrvTopics::ResponseToData(ROSData& OutMessage) const
{
	DataHelpers::Append<TArray<FString>>(OutMessage, "topics", Topics);
}

bool UROSSrvTopics::ResponseFromData(const ROSData& Message)
{
	return DataHelpers::Extract<TArray<FString>>(Message, "topics", Topics);
}
