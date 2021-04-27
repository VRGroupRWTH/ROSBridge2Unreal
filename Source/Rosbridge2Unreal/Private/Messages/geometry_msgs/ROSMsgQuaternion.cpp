#include "Messages/geometry_msgs/ROSMsgQuaternion.h"
#include "DataHelpers.h"

UROSMsgQuaternion* UROSMsgQuaternion::Create(const FQuat& Data)
{
	UROSMsgQuaternion* Message = NewObject<UROSMsgQuaternion>();
	Message->SetFromFQuat(Data);
	return Message;
}

UROSMsgQuaternion* UROSMsgQuaternion::CreateEmpty()
{
	return NewObject<UROSMsgQuaternion>();
}

FQuat UROSMsgQuaternion::AsFQuat() const
{
	return FQuat(X,Y,Z,W);
}

void UROSMsgQuaternion::SetFromFQuat(const FQuat& InQuat)
{
	X = InQuat.X;
	Y = InQuat.Y;
	Z = InQuat.Z;
	W = InQuat.W;
}

void UROSMsgQuaternion::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendDouble(OutMessage, "x", X);
	DataHelpers::AppendDouble(OutMessage, "y", Y);
	DataHelpers::AppendDouble(OutMessage, "z", Z);
	DataHelpers::AppendDouble(OutMessage, "z", W);
}

bool UROSMsgQuaternion::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractDouble(Message, "x", X)
	&& DataHelpers::ExtractDouble(Message, "y", Y)
	&& DataHelpers::ExtractDouble(Message, "z", Z)
	&& DataHelpers::ExtractDouble(Message, "w", W);
}

