// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/geometry_msgs/ROSMsgVector3BP.h"
#include "DataHelpers.h"

UROSMsgVector3BP* UROSMsgVector3BP::CreateFromVector(const FVector& Data)
{
	UROSMsgVector3BP* Message = NewObject<UROSMsgVector3BP>();
	Message->Vector = Data;
	return Message;
}

UROSMsgVector3BP* UROSMsgVector3BP::Create(const float& X, const float& Y, const float& Z)
{
	UROSMsgVector3BP* Message = NewObject<UROSMsgVector3BP>();
	Message->Vector.X = X;
	Message->Vector.Y = Y;
	Message->Vector.Z = Z;
	return Message;
}

void UROSMsgVector3BP::ToData(ROSData& Message) const
{
	DataHelpers::AppendFloat(Message, "x", Vector.X);
	DataHelpers::AppendFloat(Message, "y", Vector.Y);
	DataHelpers::AppendFloat(Message, "z", Vector.Z);
}

bool UROSMsgVector3BP::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractFloat(Message, "x", Vector.X)
	&& DataHelpers::ExtractFloat(Message, "y", Vector.Y)
	&& DataHelpers::ExtractFloat(Message, "z", Vector.Z);
}

