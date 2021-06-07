#include "Messages/internal/ROSServiceResponseMessage.h"
#include "DataHelpers.h"

void UROSServiceResponseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "service_response");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "service", ServiceName);
	DataHelpers::Append<bool>(OutMessage, "result", false); /* I got no clue what it does, but it has to be there and false */
	DataHelpers::Append<ROSData>(OutMessage, "values", Data);
}

bool UROSServiceResponseMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::Extract<FString>(Message, "id", ID);
	DataHelpers::Extract<ROSData>(Message, "values", Data);
	
	return DataHelpers::Extract<FString>(Message, "service", ServiceName);
}