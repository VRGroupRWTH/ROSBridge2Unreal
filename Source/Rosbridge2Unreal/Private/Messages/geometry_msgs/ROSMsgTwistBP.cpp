// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/geometry_msgs/ROSMsgTwistBP.h"
#include "DataHelpers.h"

UROSMsgTwistBP* UROSMsgTwistBP::Create(const float& Lx, const float& Ly, const float& Lz, const float& Ax, const float& Ay, const float& Az)
{
	UROSMsgTwistBP* Message = NewObject<UROSMsgTwistBP>();
	Message->Linear.X = Lx;
	Message->Linear.Y = Ly;
	Message->Linear.Z = Lz;
	Message->Angular.X = Ax;
	Message->Angular.Y = Ay;
	Message->Angular.Z = Az;
	return Message;
}

UROSMsgTwistBP* UROSMsgTwistBP::CreateFromVectors(const FVector& Linear, const FVector& Angular)
{
	UROSMsgTwistBP* Message = NewObject<UROSMsgTwistBP>();
	Message->Linear = Linear;
	Message->Angular = Angular;
	return Message;
}

UROSMsgTwistBP* UROSMsgTwistBP::CreateEmpty()
{
	return NewObject<UROSMsgTwistBP>();
}

void UROSMsgTwistBP::ToData(ROSData& Message) const
{
	DataHelpers::AppendSubDocument(Message, "linear", ROSData());
	DataHelpers::AppendFloat(Message, "/linear/x", Linear.X);
	DataHelpers::AppendFloat(Message, "/linear/y", Linear.Y);
	DataHelpers::AppendFloat(Message, "/linear/z", Linear.Z);
	DataHelpers::AppendSubDocument(Message, "angular", ROSData());
	DataHelpers::AppendFloat(Message, "/angular/x", Angular.X);
	DataHelpers::AppendFloat(Message, "/angular/y", Angular.Y);
	DataHelpers::AppendFloat(Message, "/angular/z", Angular.Z);
}

bool UROSMsgTwistBP::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractFloat(Message, "/linear/x", Linear.X)
	&& DataHelpers::ExtractFloat(Message, "/linear/y", Linear.Y)
	&& DataHelpers::ExtractFloat(Message, "/linear/z", Linear.Z)
	&& DataHelpers::ExtractFloat(Message, "/angular/x", Angular.X)
	&& DataHelpers::ExtractFloat(Message, "/angular/y", Angular.Y)
	&& DataHelpers::ExtractFloat(Message, "/angular/z", Angular.Z);
}
