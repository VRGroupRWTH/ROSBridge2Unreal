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

void UROSMsgPose::SetPositionFromFVector(const FVector InPosition)
{
	Px = InPosition.X;
	Py = InPosition.Y;
	Pz = InPosition.Z;
}

FQuat UROSMsgPose::OrientationAsQuad() const
{
	return FQuat(Qx,Qy,Qz,Qw);
}

void UROSMsgPose::SetOrientationFromQuad(const FQuat InOrientation)
{
	Qx = InOrientation.X;
	Qy = InOrientation.Y;
	Qz = InOrientation.Z;
	Qw = InOrientation.W;
}


void UROSMsgPose::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendSubDocument(OutMessage, "position", ROSData());
	DataHelpers::AppendDouble(OutMessage, "/position/x", Px);
	DataHelpers::AppendDouble(OutMessage, "/position/y", Py);
	DataHelpers::AppendDouble(OutMessage, "/position/z", Pz);
	DataHelpers::AppendSubDocument(OutMessage, "orientation", ROSData());
	DataHelpers::AppendDouble(OutMessage, "/orientation/x", Qx);
	DataHelpers::AppendDouble(OutMessage, "/orientation/y", Qy);
	DataHelpers::AppendDouble(OutMessage, "/orientation/z", Qz);
	DataHelpers::AppendDouble(OutMessage, "/orientation/w", Qw);
}

bool UROSMsgPose::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractDouble(Message, "/position/x", Px)
	&& DataHelpers::ExtractDouble(Message, "/position/y", Py)
	&& DataHelpers::ExtractDouble(Message, "/position/z", Pz)
	&& DataHelpers::ExtractDouble(Message, "/orientation/x", Qx)
	&& DataHelpers::ExtractDouble(Message, "/orientation/y", Qy)
	&& DataHelpers::ExtractDouble(Message, "/orientation/z", Qz)
	&& DataHelpers::ExtractDouble(Message, "/orientation/w", Qw);
}

