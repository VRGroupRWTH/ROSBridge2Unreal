#include "Messages/geometry_msgs/ROSMsgPose.h"
#include "DataHelpers.h"

UROSMsgPose* UROSMsgPose::CreateFromPositionOrientation(const FVector& Position, const FQuat& Orientation)
{
	UROSMsgPose* Message = NewObject<UROSMsgPose>();
	Message->SetPositionFromFVector(Position);
	Message->SetOrientationFromQuad(Orientation);
	return Message;
}

UROSMsgPose* UROSMsgPose::Create(double Px, double Py, double Pz, double Qx, double Qy, double Qz, double Qw)
{
	UROSMsgPose* Message = NewObject<UROSMsgPose>();
	Message->Px = Px;
	Message->Py = Py;
	Message->Pz = Pz;
	Message->Qx = Qx;
	Message->Qy = Qy;
	Message->Qz = Qz;
	Message->Qw = Qw;
	return Message;
}

FVector UROSMsgPose::PositionAsFVector() const
{
	return FVector(Px,Py,Pz);
}

void UROSMsgPose::SetPositionFromFVector(const FVector& InPosition)
{
	Px = InPosition.X;
	Py = InPosition.Y;
	Pz = InPosition.Z;
}

FQuat UROSMsgPose::OrientationAsQuad() const
{
	return FQuat(Qx,Qy,Qz,Qw);
}

void UROSMsgPose::SetOrientationFromQuad(const FQuat& InOrientation)
{
	Qx = InOrientation.X;
	Qy = InOrientation.Y;
	Qz = InOrientation.Z;
	Qw = InOrientation.W;
}


void UROSMsgPose::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<ROSData>(OutMessage, "position", ROSData());
	DataHelpers::Append<double>(OutMessage, "/position/x", Px);
	DataHelpers::Append<double>(OutMessage, "/position/y", Py);
	DataHelpers::Append<double>(OutMessage, "/position/z", Pz);
	DataHelpers::Append<ROSData>(OutMessage, "orientation", ROSData());
	DataHelpers::Append<double>(OutMessage, "/orientation/x", Qx);
	DataHelpers::Append<double>(OutMessage, "/orientation/y", Qy);
	DataHelpers::Append<double>(OutMessage, "/orientation/z", Qz);
	DataHelpers::Append<double>(OutMessage, "/orientation/w", Qw);
}

bool UROSMsgPose::FromData(const ROSData& Message)
{
	return DataHelpers::Extract<double>(Message, "/position/x", Px)
	&& DataHelpers::Extract<double>(Message, "/position/y", Py)
	&& DataHelpers::Extract<double>(Message, "/position/z", Pz)
	&& DataHelpers::Extract<double>(Message, "/orientation/x", Qx)
	&& DataHelpers::Extract<double>(Message, "/orientation/y", Qy)
	&& DataHelpers::Extract<double>(Message, "/orientation/z", Qz)
	&& DataHelpers::Extract<double>(Message, "/orientation/w", Qw);
}

