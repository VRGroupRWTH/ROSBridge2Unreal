#include "Messages/geometry_msgs/ROSMsgQuaternionBP.h"
#include "DataHelpers.h"

UROSMsgQuaternionBP* UROSMsgQuaternionBP::Create(const FQuat& Data)
{
	UROSMsgQuaternionBP* Message = NewObject<UROSMsgQuaternionBP>();
	Message->Quaternion = Data;
	return Message;
}

UROSMsgQuaternionBP* UROSMsgQuaternionBP::CreateEmpty()
{
	return NewObject<UROSMsgQuaternionBP>();
}

void UROSMsgQuaternionBP::ToData(ROSData& Message) const
{
	DataHelpers::AppendFloat(Message, "x", Quaternion.X);
	DataHelpers::AppendFloat(Message, "y", Quaternion.Y);
	DataHelpers::AppendFloat(Message, "z", Quaternion.Z);
	DataHelpers::AppendFloat(Message, "z", Quaternion.W);
}

bool UROSMsgQuaternionBP::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractFloat(Message, "x", Quaternion.X)
	&& DataHelpers::ExtractFloat(Message, "y", Quaternion.Y)
	&& DataHelpers::ExtractFloat(Message, "z", Quaternion.Z)
	&& DataHelpers::ExtractFloat(Message, "w", Quaternion.W);
}

