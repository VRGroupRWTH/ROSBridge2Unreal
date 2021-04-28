#include "Messages/internal/ROSServiceCallMessage.h"
#include "DataHelpers.h"

void UROSServiceCallMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "call_service");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "service", ServiceName);
	DataHelpers::AppendSubDocument(OutMessage, "args", Data);
}

bool UROSServiceCallMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::ExtractString(Message, "id", ID);
	DataHelpers::ExtractSubDocument(Message, "args", Data);
	
	return DataHelpers::ExtractString(Message, "service", ServiceName);
}
