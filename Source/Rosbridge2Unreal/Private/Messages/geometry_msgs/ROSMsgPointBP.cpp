#include "Messages/geometry_msgs/ROSMsgPointBP.h"
#include "DataHelpers.h"

UROSMsgPointBP* UROSMsgPointBP::CreateFromVector(const FVector& Data)
{
	UROSMsgPointBP* Message = NewObject<UROSMsgPointBP>();
	Message->Data = Data;
	return Message;
}

UROSMsgPointBP* UROSMsgPointBP::Create(const float& X, const float& Y, const float& Z)
{
	UROSMsgPointBP* Message = NewObject<UROSMsgPointBP>();
	Message->Data.X = X;
	Message->Data.Y = Y;
	Message->Data.Z = Z;
	return Message;
}

void UROSMsgPointBP::ToData(ROSData& Message) const
{
	DataHelpers::AppendFloat(Message, "x", Data.X);
	DataHelpers::AppendFloat(Message, "y", Data.Y);
	DataHelpers::AppendFloat(Message, "z", Data.Z);
}

bool UROSMsgPointBP::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractFloat(Message, "x", Data.X)
	&& DataHelpers::ExtractFloat(Message, "y", Data.Y)
	&& DataHelpers::ExtractFloat(Message, "z", Data.Z);
}
