#include "Messages/internal/ROSServiceAdvertiseMessage.h"
#include "DataHelpers.h"

void UROSServiceAdvertiseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "advertise_service");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "service", ServiceName);
	DataHelpers::Append<FString>(OutMessage, "type", ServiceType);
}

bool UROSServiceAdvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::Extract<FString>(Message, "id", ID); //optional
	return
		DataHelpers::Extract<FString>(Message, "service", ServiceName) &&
		DataHelpers::Extract<FString>(Message, "type", ServiceType);
}