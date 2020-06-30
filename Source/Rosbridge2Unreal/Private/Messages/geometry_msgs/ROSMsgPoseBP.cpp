#include "Messages/geometry_msgs/ROSMsgPoseBP.h"
#include "DataHelpers.h"

UROSMsgPoseBP* UROSMsgPoseBP::CreateFromPositionOrientation(const FVector& Position, const FQuat& Orientation)
{
	UROSMsgPoseBP* Message = NewObject<UROSMsgPoseBP>();
	Message->Position = Position;
	Message->Orientation = Orientation;
	return Message;
}

UROSMsgPoseBP* UROSMsgPoseBP::Create(const float& Px, const float& Py, const float& Pz, const float& Qx, const float& Qy, const float& Qz, const float& Qw)
{
	UROSMsgPoseBP* Message = NewObject<UROSMsgPoseBP>();
	Message->Position.X = Px;
	Message->Position.Y = Py;
	Message->Position.Z = Pz;
	Message->Orientation.X = Qx;
	Message->Orientation.Y = Qy;
	Message->Orientation.Z = Qz;
	Message->Orientation.W = Qw;
	return Message;
}

void UROSMsgPoseBP::ToData(ROSData& Message) const
{
	DataHelpers::AppendSubDocument(Message, "position", ROSData());
	DataHelpers::AppendFloat(Message, "/position/x", Position.X);
	DataHelpers::AppendFloat(Message, "/position/y", Position.Y);
	DataHelpers::AppendFloat(Message, "/position/z", Position.Z);
	DataHelpers::AppendSubDocument(Message, "orientation", ROSData());
	DataHelpers::AppendFloat(Message, "/orientation/x", Orientation.X);
	DataHelpers::AppendFloat(Message, "/orientation/y", Orientation.Y);
	DataHelpers::AppendFloat(Message, "/orientation/z", Orientation.Z);
	DataHelpers::AppendFloat(Message, "/orientation/w", Orientation.W);
}

bool UROSMsgPoseBP::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractFloat(Message, "/position/x", Position.X)
	&& DataHelpers::ExtractFloat(Message, "/position/y", Position.Y)
	&& DataHelpers::ExtractFloat(Message, "/position/z", Position.Z)
	&& DataHelpers::ExtractFloat(Message, "/orientation/x", Orientation.X)
	&& DataHelpers::ExtractFloat(Message, "/orientation/y", Orientation.Y)
	&& DataHelpers::ExtractFloat(Message, "/orientation/z", Orientation.Z)
	&& DataHelpers::ExtractFloat(Message, "/orientation/w", Orientation.W);
}

