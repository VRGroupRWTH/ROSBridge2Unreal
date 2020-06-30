#include "Messages/geometry_msgs/ROSMsgPose.h"
#include "DataHelpers.h"

UROSMsgPose* UROSMsgPose::CreateFromPositionOrientation(const FVector& Position, const FQuat& Orientation)
{
	UROSMsgPose* Message = NewObject<UROSMsgPose>();
	Message->Px = Position.X;
	Message->Py = Position.Y;
	Message->Pz = Position.Z;
	Message->Qx = Orientation.X;
	Message->Qy = Orientation.Y;
	Message->Qz = Orientation.Z;
	Message->Qw = Orientation.W;
	return Message;
}

UROSMsgPose* UROSMsgPose::Create(const double& Px, const double& Py, const double& Pz, const double& Qx, const double& Qy, const double& Qz, const double& Qw)
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

FQuat UROSMsgPose::OrientationAsQuad() const
{
	return FQuat(Qx,Qy,Qz,Qw);
}

void UROSMsgPose::ToData(ROSData& Message) const
{
	DataHelpers::AppendSubDocument(Message, "position", ROSData());
	DataHelpers::AppendDouble(Message, "/position/x", Px);
	DataHelpers::AppendDouble(Message, "/position/y", Py);
	DataHelpers::AppendDouble(Message, "/position/z", Pz);
	DataHelpers::AppendSubDocument(Message, "orientation", ROSData());
	DataHelpers::AppendDouble(Message, "/orientation/x", Qx);
	DataHelpers::AppendDouble(Message, "/orientation/y", Qy);
	DataHelpers::AppendDouble(Message, "/orientation/z", Qz);
	DataHelpers::AppendDouble(Message, "/orientation/w", Qw);
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

