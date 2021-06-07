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
	DataHelpers::Append<double>(OutMessage, "x", X);
	DataHelpers::Append<double>(OutMessage, "y", Y);
	DataHelpers::Append<double>(OutMessage, "z", Z);
}

bool UROSMsgPoint::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<double>(Message, "x", X) &&
		DataHelpers::Extract<double>(Message, "y", Y) &&
		DataHelpers::Extract<double>(Message, "z", Z);
}
