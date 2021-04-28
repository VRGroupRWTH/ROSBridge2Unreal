#include "Messages/internal/ROSTopicPublishMessage.h"
#include "DataHelpers.h"

void UROSTopicPublishMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "publish");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "topic", TopicName);
	DataHelpers::AppendSubDocument(OutMessage, "msg", Data);
}

bool UROSTopicPublishMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "topic", TopicName)
	&& DataHelpers::ExtractSubDocument(Message, "msg", Data);
}