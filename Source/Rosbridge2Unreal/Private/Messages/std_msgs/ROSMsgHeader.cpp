// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/std_msgs/ROSMsgHeader.h"
#include "DataHelpers.h"

UROSMsgHeader* UROSMsgHeader::CreateWithGeneratedTimeStamp(FString FrameID, int64 SequenceID)
{
	UROSMsgHeader* Message = NewObject<UROSMsgHeader>();
	Message->SequenceID = SequenceID;
	Message->FrameID = FrameID;

	//Using "Wallclock" time since epoch
	const FTimespan WallClockTime = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(0);
	Message->Seconds = static_cast<int32>(WallClockTime.GetTotalSeconds()); //Implicit floor
	Message->NanoSeconds = WallClockTime.GetFractionNano();

	return Message;
}

UROSMsgHeader* UROSMsgHeader::Create(FString FrameID, int64 SequenceID, int32 Seconds, int32 NanoSeconds)
{
	UROSMsgHeader* Message = NewObject<UROSMsgHeader>();
	Message->SequenceID = SequenceID;
	Message->FrameID = FrameID;
	Message->Seconds = Seconds;
	Message->NanoSeconds = NanoSeconds;

	return Message;
}

void UROSMsgHeader::ToData(ROSData& Message) const
{
	DataHelpers::AppendUInt32(Message, "seq", SequenceID);
	DataHelpers::AppendString(Message, "frame_id", FrameID);
	DataHelpers::AppendSubDocument(Message, "stamp", ROSData());
	DataHelpers::AppendInt32(Message, "/stamp/secs", Seconds);
	DataHelpers::AppendInt32(Message, "/stamp/nsecs", NanoSeconds);
}

bool UROSMsgHeader::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractInt64(Message, "seq", SequenceID)
	&& DataHelpers::ExtractString(Message, "frame_id", FrameID)
	&& DataHelpers::ExtractInt32(Message, "/stamp/secs", Seconds)
	&& DataHelpers::ExtractInt32(Message, "/stamp/nsecs", NanoSeconds);
}
