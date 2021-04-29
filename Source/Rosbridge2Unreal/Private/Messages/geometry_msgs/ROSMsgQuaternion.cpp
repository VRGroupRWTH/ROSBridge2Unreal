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
	DataHelpers::Append<double>(OutMessage, "x", X);
	DataHelpers::Append<double>(OutMessage, "y", Y);
	DataHelpers::Append<double>(OutMessage, "z", Z);
	DataHelpers::Append<double>(OutMessage, "z", W);
}

bool UROSMsgQuaternion::FromData(const ROSData& Message)
{
	return DataHelpers::Extract<double>(Message, "x", X)
	&& DataHelpers::Extract<double>(Message, "y", Y)
	&& DataHelpers::Extract<double>(Message, "z", Z)
	&& DataHelpers::Extract<double>(Message, "w", W);
}

