#include "Messages/internal/ROSServiceCallMessage.h"
#include "DataHelpers.h"

void UROSServiceCallMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "call_service");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "service", ServiceName);
	DataHelpers::Append<ROSData>(OutMessage, "args", Data);
}

bool UROSServiceCallMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::Extract<FString>(Message, "id", ID);
	DataHelpers::Extract<ROSData>(Message, "args", Data);
	
	return DataHelpers::Extract<FString>(Message, "service", ServiceName);
}
