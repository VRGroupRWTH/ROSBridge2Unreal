#include "Messages/geometry_msgs/ROSMsgTwist.h"
#include "DataHelpers.h"

UROSMsgTwist* UROSMsgTwist::Create(double Lx, double Ly, double Lz, double Ax, double Ay, double Az)
{
	UROSMsgTwist* Message = NewObject<UROSMsgTwist>();
	Message->LinearX = Lx;
	Message->LinearY = Ly;
	Message->LinearZ = Lz;
	Message->AngularX = Ax;
	Message->AngularY = Ay;
	Message->AngularZ = Az;
	return Message;
}

UROSMsgTwist* UROSMsgTwist::CreateFromVectors(const FVector& InLinear, const FVector& InAngular)
{
	UROSMsgTwist* Message = NewObject<UROSMsgTwist>();
	Message->SetLinearFromFVector(InLinear);
	Message->SetAngularFromFVector(InAngular);
	return Message;
}

UROSMsgTwist* UROSMsgTwist::CreateEmpty()
{
	return NewObject<UROSMsgTwist>();
}

FVector UROSMsgTwist::LinearAsFVector() const
{
	return FVector(LinearX, LinearY, LinearZ);
}

FVector UROSMsgTwist::AngularAsFVector() const
{
	return FVector(AngularX, AngularY, AngularZ);
}

void UROSMsgTwist::SetLinearFromFVector(const FVector InVector)
{
	LinearX = InVector.X;
	LinearY = InVector.Y;
	LinearZ = InVector.Z;
}

void UROSMsgTwist::SetAngularFromFVector(const FVector InVector)
{
	AngularX = InVector.X;
	AngularY = InVector.Y;
	AngularZ = InVector.Z;
}

void UROSMsgTwist::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendSubDocument(OutMessage, "linear", ROSData());
	DataHelpers::AppendDouble(OutMessage, "/linear/x", LinearX);
	DataHelpers::AppendDouble(OutMessage, "/linear/y", LinearY);
	DataHelpers::AppendDouble(OutMessage, "/linear/z", LinearZ);
	DataHelpers::AppendSubDocument(OutMessage, "angular", ROSData());
	DataHelpers::AppendDouble(OutMessage, "/angular/x", AngularX);
	DataHelpers::AppendDouble(OutMessage, "/angular/y", AngularY);
	DataHelpers::AppendDouble(OutMessage, "/angular/z", AngularZ);
}

bool UROSMsgTwist::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractDouble(Message, "/linear/x", LinearX)
	&& DataHelpers::ExtractDouble(Message, "/linear/y", LinearY)
	&& DataHelpers::ExtractDouble(Message, "/linear/z", LinearZ)
	&& DataHelpers::ExtractDouble(Message, "/angular/x", AngularX)
	&& DataHelpers::ExtractDouble(Message, "/angular/y", AngularY)
	&& DataHelpers::ExtractDouble(Message, "/angular/z", AngularZ);
}
