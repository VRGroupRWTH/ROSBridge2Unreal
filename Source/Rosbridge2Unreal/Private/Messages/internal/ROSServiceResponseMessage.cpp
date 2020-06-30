#include "Messages/internal/ROSServiceResponseMessage.h"
#include "DataHelpers.h"

void UROSServiceResponseMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "service_response");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "service", ServiceName);
	DataHelpers::AppendBool(Message, "result", false); /* I got no clue what it does, but it has to be there and false */
	DataHelpers::AppendSubDocument(Message, "values", Data);
}

bool UROSServiceResponseMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::ExtractString(Message, "id", ID);
	DataHelpers::ExtractSubDocument(Message, "values", Data);
	
	return DataHelpers::ExtractString(Message, "service", ServiceName);
}