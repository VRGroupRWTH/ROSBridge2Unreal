#include "Messages/internal/ROSServiceResponseMessage.h"
#include "DataHelpers.h"

void UROSServiceResponseMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "service_response");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "service", ServiceName);
	DataHelpers::AppendBool(OutMessage, "result", false); /* I got no clue what it does, but it has to be there and false */
	DataHelpers::AppendSubDocument(OutMessage, "values", Data);
}

bool UROSServiceResponseMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::ExtractString(Message, "id", ID);
	DataHelpers::ExtractSubDocument(Message, "values", Data);
	
	return DataHelpers::ExtractString(Message, "service", ServiceName);
}