// Fill out your copyright notice in the Description page of Project Settings.

#include "Services/rospy_tutorials/ROSSrvAddTwoInts.h"
#include "DataHelpers.h"

UROSSrvAddTwoInts * UROSSrvAddTwoInts::CreateRequest(int64 A, int64 B)
{
	UROSSrvAddTwoInts* Request = NewObject<UROSSrvAddTwoInts>();
	Request->A = A;
	Request->B = B;
	return Request;
}

int UROSSrvAddTwoInts::SumAsInt() const
{
	return Sum;
}

void UROSSrvAddTwoInts::RequestToData(ROSData& Message) const
{
	DataHelpers::AppendInt64(Message, "a", A);
	DataHelpers::AppendInt64(Message, "b", B);
}

bool UROSSrvAddTwoInts::RequestFromData(const ROSData& Message)
{
	return	DataHelpers::ExtractInt64(Message, "a", A)
		&& DataHelpers::ExtractInt64(Message, "b", B);
}

void UROSSrvAddTwoInts::ResponseToData(ROSData& Message) const
{
	DataHelpers::AppendInt64(Message, "sum", Sum);
}

bool UROSSrvAddTwoInts::ResponseFromData(const ROSData& Message)
{
	return DataHelpers::ExtractInt64(Message, "sum", Sum);
}
