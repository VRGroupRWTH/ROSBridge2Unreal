// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgPoint.h"
#include "DataHelpers.h"

UROSMsgPoint* UROSMsgPoint::CreateFromVector(const FVector& Data)
{
	UROSMsgPoint* Message = NewObject<UROSMsgPoint>();
	Message->X = Data.X;
	Message->Y = Data.Y;
	Message->Z = Data.Z;
	return Message;
}

UROSMsgPoint* UROSMsgPoint::Create(const double& X, const double& Y, const double& Z)
{
	UROSMsgPoint* Message = NewObject<UROSMsgPoint>();
	Message->X = X;
	Message->Y = Y;
	Message->Z = Z;
	return Message;
}

FVector UROSMsgPoint::AsFVector() const
{
	return FVector(X,Y,Z);
}

void UROSMsgPoint::ToData(ROSData& Message) const
{
	DataHelpers::AppendDouble(Message, "x", X);
	DataHelpers::AppendDouble(Message, "y", Y);
	DataHelpers::AppendDouble(Message, "z", Z);
}

bool UROSMsgPoint::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractDouble(Message, "x", X)
	&& DataHelpers::ExtractDouble(Message, "y", Y)
	&& DataHelpers::ExtractDouble(Message, "z", Z);
}
