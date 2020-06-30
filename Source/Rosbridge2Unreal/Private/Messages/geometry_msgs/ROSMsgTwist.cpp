#include "Messages/geometry_msgs/ROSMsgTwist.h"
#include "DataHelpers.h"

UROSMsgTwist* UROSMsgTwist::Create(const double& Lx, const double& Ly, const double& Lz, const double& Ax, const double& Ay, const double& Az)
{
	UROSMsgTwist* Message = NewObject<UROSMsgTwist>();
	Message->LinearX = Lx;
	Message->LinearY = Ly;
	Message->LinearZ = Lz;
	Message->AngularX = Ax;
	Message->AngularY = Ay;
	Message->AngularZ = Az;
	return Message;
}

UROSMsgTwist* UROSMsgTwist::CreateEmpty()
{
	return NewObject<UROSMsgTwist>();
}

FVector UROSMsgTwist::LinearAsFVector() const
{
	return FVector(LinearX, LinearY, LinearZ);
}

FVector UROSMsgTwist::AngularAsFVector() const
{
	return FVector(AngularX, AngularY, AngularZ);
}

void UROSMsgTwist::ToData(ROSData& Message) const
{
	DataHelpers::AppendSubDocument(Message, "linear", ROSData());
	DataHelpers::AppendDouble(Message, "/linear/x", LinearX);
	DataHelpers::AppendDouble(Message, "/linear/y", LinearY);
	DataHelpers::AppendDouble(Message, "/linear/z", LinearZ);
	DataHelpers::AppendSubDocument(Message, "angular", ROSData());
	DataHelpers::AppendDouble(Message, "/angular/x", AngularX);
	DataHelpers::AppendDouble(Message, "/angular/y", AngularY);
	DataHelpers::AppendDouble(Message, "/angular/z", AngularZ);
}

bool UROSMsgTwist::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractDouble(Message, "/linear/x", LinearX)
	&& DataHelpers::ExtractDouble(Message, "/linear/y", LinearY)
	&& DataHelpers::ExtractDouble(Message, "/linear/z", LinearZ)
	&& DataHelpers::ExtractDouble(Message, "/angular/x", AngularX)
	&& DataHelpers::ExtractDouble(Message, "/angular/y", AngularY)
	&& DataHelpers::ExtractDouble(Message, "/angular/z", AngularZ);
}
