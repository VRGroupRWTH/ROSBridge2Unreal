#include "Services/rosapi/ROSSrvServices.h"
#include "DataHelpers.h"

UROSSrvServices* UROSSrvServices::CreateEmpty()
{
	return NewObject<UROSSrvServices>();
}

void UROSSrvServices::RequestToData(ROSData& OutMessage) const {}

bool UROSSrvServices::RequestFromData(const ROSData& Message) {return true;}

void UROSSrvServices::ResponseToData(ROSData& OutMessage) const
{
	DataHelpers::Append<TArray<FString>>(OutMessage, "services", Services);
}

bool UROSSrvServices::ResponseFromData(const ROSData& Message)
{
	return DataHelpers::Extract<TArray<FString>>(Message, "services", Services);
}
