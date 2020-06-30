#include "Messages/internal/ROSServiceCallMessage.h"
#include "DataHelpers.h"

void UROSServiceCallMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "call_service");
	DataHelpers::AppendString(Message, "id", ID);
	DataHelpers::AppendString(Message, "service", ServiceName);
	DataHelpers::AppendSubDocument(Message, "args", Data);
}

bool UROSServiceCallMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::ExtractString(Message, "id", ID);
	DataHelpers::ExtractSubDocument(Message, "args", Data);
	
	return DataHelpers::ExtractString(Message, "service", ServiceName);
}
