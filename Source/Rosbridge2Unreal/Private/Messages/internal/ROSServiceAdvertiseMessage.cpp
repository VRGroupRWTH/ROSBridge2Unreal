#include "Messages/internal/ROSServiceAdvertiseMessage.h"
#include "DataHelpers.h"

void UROSServiceAdvertiseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "advertise_service");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "service", ServiceName);
	DataHelpers::AppendString(OutMessage, "type", ServiceType);
}

bool UROSServiceAdvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "service", ServiceName)
	&& DataHelpers::ExtractString(Message, "type", ServiceType);
}