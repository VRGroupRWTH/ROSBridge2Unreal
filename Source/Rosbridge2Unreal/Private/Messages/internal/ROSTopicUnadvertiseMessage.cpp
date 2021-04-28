#include "Messages/internal/ROSTopicUnadvertiseMessage.h"
#include "DataHelpers.h"

void UROSTopicUnadvertiseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "unadvertise");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "topic", TopicName);
}

bool UROSTopicUnadvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName);
}