#include "Messages/internal/ROSTopicSubscribeMessage.h"
#include "DataHelpers.h"

void UROSTopicSubscribeMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "subscribe");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "topic", TopicName);
	DataHelpers::AppendString(Message, "type", MessageType);
	DataHelpers::AppendInt32(Message, "throttle_rate", ThrottleRate);
	DataHelpers::AppendInt32(Message, "queue_length", QueueLength);
	DataHelpers::AppendString(Message, "compression", Compression);
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