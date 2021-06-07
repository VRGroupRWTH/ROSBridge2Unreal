#include "Messages/internal/ROSServiceCallMessage.h"
#include "DataHelpers.h"

void UROSServiceCallMessage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<FString>(OutMessage, "op", "call_service");
	DataHelpers::Append<FString>(OutMessage, "id", ID);
	DataHelpers::Append<FString>(OutMessage, "service", ServiceName);
	DataHelpers::Append<ROSData>(OutMessage, "args", Data);
	if(FragmentSize > 0)
	{
		DataHelpers::Append<int32>(OutMessage, "fragment_size", FragmentSize);
	}
}

bool UROSServiceCallMessage::FromData(const ROSData& Message)
{
	//optional
	DataHelpers::Extract<FString>(Message, "id", ID);
	DataHelpers::Extract<ROSData>(Message, "args", Data);
	DataHelpers::Extract<int32>(Message, "fragment_size", FragmentSize);

	return DataHelpers::Extract<FString>(Message, "service", ServiceName);
}
