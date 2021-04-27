#include "Messages/geometry_msgs/ROSMsgPoint.h"
#include "DataHelpers.h"

UROSMsgPoint* UROSMsgPoint::CreateFromVector(const FVector& Data)
{
	UROSMsgPoint* Message = NewObject<UROSMsgPoint>();
	Message->SetFromFVector(Data);
	return Message;
}

UROSMsgPoint* UROSMsgPoint::Create(double X, double Y, double Z)
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

void UROSMsgPoint::SetFromFVector(const FVector& InVector)
{
	X = InVector.X;
	Y = InVector.Y;
	Z = InVector.Z;
}

void UROSMsgPoint::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendDouble(OutMessage, "x", X);
	DataHelpers::AppendDouble(OutMessage, "y", Y);
	DataHelpers::AppendDouble(OutMessage, "z", Z);
}

bool UROSMsgPoint::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractDouble(Message, "x", X)
	&& DataHelpers::ExtractDouble(Message, "y", Y)
	&& DataHelpers::ExtractDouble(Message, "z", Z);
}
