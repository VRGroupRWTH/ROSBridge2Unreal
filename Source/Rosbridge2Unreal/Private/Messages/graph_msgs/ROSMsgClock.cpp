#include "Messages/graph_msgs/ROSMsgClock.h"
#include "DataHelpers.h"

UROSMsgClock* UROSMsgClock::Create(int32 InSeconds, int32 InNanoSeconds)
{
	UROSMsgClock* Message = NewObject<UROSMsgClock>();
	Message->Seconds = InSeconds;
	Message->NanoSeconds = InNanoSeconds;
	return Message;
}

void UROSMsgClock::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<ROSData>(OutMessage, "clock", ROSData());
	DataHelpers::Append<int32>(OutMessage, "/clock/secs", Seconds);
	DataHelpers::Append<int32>(OutMessage, "/clock/nsecs", NanoSeconds);
}

bool UROSMsgClock::FromData(const ROSData& Message)
{
	return DataHelpers::Extract<int32>(Message, "/clock/secs", Seconds)
	&& DataHelpers::Extract<int32>(Message, "/clock/nsecs", NanoSeconds);
}