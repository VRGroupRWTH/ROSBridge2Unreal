#include "Messages/internal/ROSServiceUnadvertiseMessage.h"
#include "DataHelpers.h"

void UROSServiceUnadvertiseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "unadvertise_service");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "service", ServiceName);
}

bool UROSServiceUnadvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	return DataHelpers::ExtractString(Message, "service", ServiceName);
}