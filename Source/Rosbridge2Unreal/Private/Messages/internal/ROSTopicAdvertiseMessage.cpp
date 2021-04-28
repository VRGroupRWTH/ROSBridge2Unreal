#include "Messages/internal/ROSTopicAdvertiseMessage.h"
#include "DataHelpers.h"

void UROSTopicAdvertiseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "advertise");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "topic", TopicName);
	DataHelpers::AppendString(OutMessage, "type", MessageType);
}

bool UROSTopicAdvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName)
	&& DataHelpers::ExtractString(Message, "type", MessageType);
}