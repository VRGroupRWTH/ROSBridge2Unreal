#include "Messages/internal/ROSTopicUnsubscribeMessage.h"
#include "DataHelpers.h"

void UROSTopicUnsubscribeMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "unsubscribe");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "topic", TopicName);
}

bool UROSTopicUnsubscribeMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName);
}