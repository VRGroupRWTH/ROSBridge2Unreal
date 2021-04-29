#include "Messages/internal/ROSTopicPublishMessage.h"
#include "DataHelpers.h"

void UROSTopicPublishMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "publish");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "topic", TopicName);
	DataHelpers::Append<ROSData>(OutMessage, "msg", Data);
}

bool UROSTopicPublishMessage::FromData(const ROSData& Message)
{
	DataHelpers::Extract<FString>(Message, "id", ID); //optional
	return DataHelpers::Extract<FString>(Message, "topic", TopicName)
	&& DataHelpers::Extract<ROSData>(Message, "msg", Data);
}