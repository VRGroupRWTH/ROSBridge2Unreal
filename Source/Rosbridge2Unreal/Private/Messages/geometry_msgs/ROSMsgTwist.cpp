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

void UROSMsgTwist::SetLinearFromFVector(const FVector& InVector)
{
	LinearX = InVector.X;
	LinearY = InVector.Y;
	LinearZ = InVector.Z;
}

void UROSMsgTwist::SetAngularFromFVector(const FVector& InVector)
{
	AngularX = InVector.X;
	AngularY = InVector.Y;
	AngularZ = InVector.Z;
}

void UROSMsgTwist::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<ROSData>(OutMessage, "linear", ROSData());
	DataHelpers::Append<double>(OutMessage, "/linear/x", LinearX);
	DataHelpers::Append<double>(OutMessage, "/linear/y", LinearY);
	DataHelpers::Append<double>(OutMessage, "/linear/z", LinearZ);
	DataHelpers::Append<ROSData>(OutMessage, "angular", ROSData());
	DataHelpers::Append<double>(OutMessage, "/angular/x", AngularX);
	DataHelpers::Append<double>(OutMessage, "/angular/y", AngularY);
	DataHelpers::Append<double>(OutMessage, "/angular/z", AngularZ);
}

bool UROSMsgTwist::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<double>(Message, "/linear/x", LinearX) &&
		DataHelpers::Extract<double>(Message, "/linear/y", LinearY) &&
		DataHelpers::Extract<double>(Message, "/linear/z", LinearZ) &&
		DataHelpers::Extract<double>(Message, "/angular/x", AngularX) &&
		DataHelpers::Extract<double>(Message, "/angular/y", AngularY) &&
		DataHelpers::Extract<double>(Message, "/angular/z", AngularZ);
}
