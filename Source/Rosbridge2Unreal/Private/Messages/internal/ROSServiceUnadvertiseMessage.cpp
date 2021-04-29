#include "Messages/internal/ROSServiceUnadvertiseMessage.h"
#include "DataHelpers.h"

void UROSServiceUnadvertiseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "unadvertise_service");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "service", ServiceName);
}

bool UROSServiceUnadvertiseMessage::FromData(const ROSData& Message)
{
	DataHelpers::Extract<FString>(Message, "id", ID); //optional
	return DataHelpers::Extract<FString>(Message, "service", ServiceName);
}