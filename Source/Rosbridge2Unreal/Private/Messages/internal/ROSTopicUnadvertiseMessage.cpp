#include "Messages/internal/ROSTopicUnadvertiseMessage.h"
#include "DataHelpers.h"

void UROSTopicUnadvertiseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "unadvertise");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "topic", TopicName);
}

bool UROSTopicUnadvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::Extract<FString>(Message, "id", ID); //optional
	return DataHelpers::Extract<FString>(Message, "topic", TopicName);
}