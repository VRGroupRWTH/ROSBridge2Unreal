// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgClock.h"
#include "DataHelpers.h"
#include "Engine/World.h"

UROSMsgClock* UROSMsgClock::Create(int32 InSeconds, int32 InNanoSeconds)
{
	UROSMsgClock* Message = NewObject<UROSMsgClock>();
	Message->Seconds = InSeconds;
	Message->NanoSeconds = InNanoSeconds;
	return Message;
}

void UROSMsgClock::ToData(ROSData& Message) const
{
	DataHelpers::AppendSubDocument(Message, "clock", ROSData());
	DataHelpers::AppendInt32(Message, "/clock/secs", Seconds);
	DataHelpers::AppendInt32(Message, "/clock/nsecs", NanoSeconds);
}

bool UROSMsgClock::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractInt32(Message, "/clock/secs", Seconds)
	&& DataHelpers::ExtractInt32(Message, "/clock/nsecs", NanoSeconds);
}