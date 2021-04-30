#include "Messages/internal/ROSServiceCallMessage.h"
#include "DataHelpers.h"

void UROSServiceCallMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendString(OutMessage, "op", "call_service");
	DataHelpers::AppendString(OutMessage, "id", ID);
	DataHelpers::AppendString(OutMessage, "service", ServiceName);
	DataHelpers::AppendSubDocument(OutMessage, "args", Data);
	if(FragmentSize > 0)
	{
		DataHelpers::AppendInt32(OutMessage, "fragment_size", FragmentSize);
	}
}

bool UROSServiceCallMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::ExtractString(Message, "id", ID);
	DataHelpers::ExtractSubDocument(Message, "args", Data);
	DataHelpers::ExtractInt32(Message, "fragment_size", FragmentSize);
	
	return DataHelpers::ExtractString(Message, "service", ServiceName);
}
