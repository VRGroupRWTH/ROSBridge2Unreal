#include "Services/rospy_tutorials/ROSSrvAddTwoInts.h"
#include "DataHelpers.h"

UROSSrvAddTwoInts * UROSSrvAddTwoInts::CreateRequest(int64 A, int64 B)
{
	UROSSrvAddTwoInts* Request = NewObject<UROSSrvAddTwoInts>();
	Request->A = A;
	Request->B = B;
	return Request;
}

UROSSrvAddTwoInts* UROSSrvAddTwoInts::CreateEmpty()
{
	return NewObject<UROSSrvAddTwoInts>();
}

int UROSSrvAddTwoInts::SumAsInt() const
{
	return Sum;
}

void UROSSrvAddTwoInts::RequestToData(ROSData& OutMessage) const
{
	DataHelpers::AppendInt64(OutMessage, "a", A);
	DataHelpers::AppendInt64(OutMessage, "b", B);
}

bool UROSSrvAddTwoInts::RequestFromData(const ROSData& Message)
{
	return	DataHelpers::ExtractInt64(Message, "a", A)
		&& DataHelpers::ExtractInt64(Message, "b", B);
}

void UROSSrvAddTwoInts::ResponseToData(ROSData& OutMessage) const
{
	DataHelpers::AppendInt64(OutMessage, "sum", Sum);
}

bool UROSSrvAddTwoInts::ResponseFromData(const ROSData& Message)
{
	return DataHelpers::ExtractInt64(Message, "sum", Sum);
}
