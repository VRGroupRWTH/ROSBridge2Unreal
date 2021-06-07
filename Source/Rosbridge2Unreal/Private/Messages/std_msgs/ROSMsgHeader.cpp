#include "Messages/std_msgs/ROSMsgHeader.h"
#include "DataHelpers.h"

UROSMsgHeader* UROSMsgHeader::CreateWithGeneratedTimeStamp(const FString& FrameID, int64 SequenceID)
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

UROSMsgHeader* UROSMsgHeader::Create(const FString& FrameID, int64 SequenceID, int32 Seconds, int32 NanoSeconds)
{
	UROSMsgHeader* Message = NewObject<UROSMsgHeader>();
	Message->SequenceID = SequenceID;
	Message->FrameID = FrameID;
	Message->Seconds = Seconds;
	Message->NanoSeconds = NanoSeconds;

	return Message;
}

void UROSMsgHeader::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<uint32>(OutMessage, "seq", SequenceID);
	DataHelpers::Append<ROSData>(OutMessage, "stamp", ROSData());
	DataHelpers::Append<int32>(OutMessage, "/stamp/secs", Seconds);
	DataHelpers::Append<int32>(OutMessage, "/stamp/nsecs", NanoSeconds);
	DataHelpers::Append<FString>(OutMessage, "frame_id", FrameID);
}

bool UROSMsgHeader::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<int64>(Message, "seq", SequenceID) &&
		DataHelpers::Extract<FString>(Message, "frame_id", FrameID) &&
		DataHelpers::Extract<int32>(Message, "/stamp/secs", Seconds) &&
		DataHelpers::Extract<int32>(Message, "/stamp/nsecs", NanoSeconds);
}
