#include "Messages/internal/ROSTopicAdvertiseMessage.h"
#include "DataHelpers.h"

void UROSTopicAdvertiseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "advertise");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "topic", TopicName);
	DataHelpers::Append<FString>(OutMessage, "type", MessageType);
}

bool UROSTopicAdvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::Extract<FString>(Message, "id", ID); //optional
	return DataHelpers::Extract<FString>(Message, "topic", TopicName)
	&& DataHelpers::Extract<FString>(Message, "type", MessageType);
}