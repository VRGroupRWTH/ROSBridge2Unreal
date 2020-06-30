#include "Messages/internal/ROSTopicUnsubscribeMessage.h"
#include "DataHelpers.h"

void UROSTopicUnsubscribeMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "unsubscribe");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "topic", TopicName);
}

bool UROSTopicUnsubscribeMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName);
}