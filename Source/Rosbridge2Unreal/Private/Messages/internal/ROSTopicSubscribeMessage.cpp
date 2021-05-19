#include "Messages/internal/ROSTopicSubscribeMessage.h"
#include "DataHelpers.h"

void UROSTopicSubscribeMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "subscribe");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "topic", TopicName);
	DataHelpers::Append<FString>(OutMessage, "type", MessageType);
	DataHelpers::Append<int32>(OutMessage, "throttle_rate", ThrottleRate);
	DataHelpers::Append<int32>(OutMessage, "queue_length", QueueLength);
	DataHelpers::Append<FString>(OutMessage, "compression", Compression);
	if(FragmentSize > 0)
	{
		DataHelpers::Append<int32>(OutMessage, "fragment_size", FragmentSize);
	}
}

bool UROSTopicSubscribeMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::Extract<FString>(Message, "id", ID);
	DataHelpers::Extract<FString>(Message, "type", MessageType);
	DataHelpers::Extract<int32>(Message, "throttle_rate", ThrottleRate);
	DataHelpers::Extract<int32>(Message, "queue_length", QueueLength);
	DataHelpers::Extract<FString>(Message, "compression", Compression);
	
	return DataHelpers::Extract<FString>(Message, "topic", TopicName);
}