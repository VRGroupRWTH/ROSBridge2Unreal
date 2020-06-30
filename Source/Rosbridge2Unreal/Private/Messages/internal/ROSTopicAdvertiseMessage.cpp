#include "Messages/internal/ROSTopicAdvertiseMessage.h"
#include "DataHelpers.h"

void UROSTopicAdvertiseMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "advertise");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "topic", TopicName);
	DataHelpers::AppendString(Message, "type", MessageType);
}

bool UROSTopicAdvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName)
	&& DataHelpers::ExtractString(Message, "type", MessageType);
}