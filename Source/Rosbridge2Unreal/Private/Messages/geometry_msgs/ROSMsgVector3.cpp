#include "Messages/geometry_msgs/ROSMsgVector3.h"
#include "DataHelpers.h"

UROSMsgVector3* UROSMsgVector3::CreateFromVector(const FVector& Data)
{
	UROSMsgVector3* Message = NewObject<UROSMsgVector3>();
	Message->SetFromFVector(Data);
	return Message;
}

UROSMsgVector3* UROSMsgVector3::Create(const double& X, const double& Y, const double& Z)
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

void UROSMsgVector3::SetFromFVector(const FVector InVector)
{
	X = InVector.X;
	Y = InVector.Y;
	Z = InVector.Z;
}

void UROSMsgVector3::ToData(ROSData& Message) const
{
	DataHelpers::AppendDouble(Message, "x", X);
	DataHelpers::AppendDouble(Message, "y", Y);
	DataHelpers::AppendDouble(Message, "z", Z);
}

bool UROSMsgVector3::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractDouble(Message, "x", X)
	&& DataHelpers::ExtractDouble(Message, "y", Y)
	&& DataHelpers::ExtractDouble(Message, "z", Z);
}

