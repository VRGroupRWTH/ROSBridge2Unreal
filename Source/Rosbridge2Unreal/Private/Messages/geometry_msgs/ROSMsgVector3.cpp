#include "Messages/geometry_msgs/ROSMsgVector3.h"
#include "DataHelpers.h"

UROSMsgVector3* UROSMsgVector3::CreateFromVector(const FVector& Data)
{
	UROSMsgVector3* Message = NewObject<UROSMsgVector3>();
	Message->SetFromFVector(Data);
	return Message;
}

UROSMsgVector3* UROSMsgVector3::Create(double X, double Y, double Z)
{
	UROSMsgVector3* Message = NewObject<UROSMsgVector3>();
	Message->X = X;
	Message->Y = Y;
	Message->Z = Z;
	return Message;
}

FVector UROSMsgVector3::AsFVector() const
{
	return FVector(X,Y,Z);
}

void UROSMsgVector3::SetFromFVector(const FVector& InVector)
{
	X = InVector.X;
	Y = InVector.Y;
	Z = InVector.Z;
}

void UROSMsgVector3::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<double>(OutMessage, "x", X);
	DataHelpers::Append<double>(OutMessage, "y", Y);
	DataHelpers::Append<double>(OutMessage, "z", Z);
}

bool UROSMsgVector3::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<double>(Message, "x", X) &&
		DataHelpers::Extract<double>(Message, "y", Y) &&
		DataHelpers::Extract<double>(Message, "z", Z);
}

