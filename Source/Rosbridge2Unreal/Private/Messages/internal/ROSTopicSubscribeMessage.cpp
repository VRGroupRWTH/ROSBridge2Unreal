#include "Messages/internal/ROSTopicSubscribeMessage.h"
#include "DataHelpers.h"

void UROSTopicSubscribeMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "subscribe");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "topic", TopicName);
	DataHelpers::AppendString(OutMessage, "type", MessageType);
	DataHelpers::AppendInt32(OutMessage, "throttle_rate", ThrottleRate);
	DataHelpers::AppendInt32(OutMessage, "queue_length", QueueLength);
	DataHelpers::AppendString(OutMessage, "compression", Compression);
}

bool UROSTopicSubscribeMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::ExtractString(Message, "id", ID);
	DataHelpers::ExtractString(Message, "type", MessageType);
	DataHelpers::ExtractInt32(Message, "throttle_rate", ThrottleRate);
	DataHelpers::ExtractInt32(Message, "queue_length", QueueLength);
	DataHelpers::ExtractString(Message, "compression", Compression);
	
	return DataHelpers::ExtractString(Message, "topic", TopicName);
}