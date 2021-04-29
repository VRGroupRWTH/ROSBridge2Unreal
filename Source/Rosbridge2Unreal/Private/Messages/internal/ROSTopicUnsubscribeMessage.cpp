#include "Messages/internal/ROSTopicUnsubscribeMessage.h"
#include "DataHelpers.h"

void UROSTopicUnsubscribeMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "unsubscribe");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "topic", TopicName);
}

bool UROSTopicUnsubscribeMessage::FromData(const ROSData& Message)
{
	DataHelpers::Extract<FString>(Message, "id", ID); //optional
	return DataHelpers::Extract<FString>(Message, "topic", TopicName);
}