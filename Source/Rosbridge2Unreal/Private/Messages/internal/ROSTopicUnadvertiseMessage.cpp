#include "Messages/internal/ROSTopicUnadvertiseMessage.h"
#include "DataHelpers.h"

void UROSTopicUnadvertiseMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "unadvertise");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "topic", TopicName);
}

bool UROSTopicUnadvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName);
}