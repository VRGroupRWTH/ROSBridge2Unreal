// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/geometry_msgs/ROSMsgTwistWithCovariance.h"
#include "DataHelpers.h"
#include "LogCategory.h"

UROSMsgTwistWithCovariance* UROSMsgTwistWithCovariance::Create(UROSMsgTwist* Twist, const TArray<double>& Covariance)
{
	UROSMsgTwistWithCovariance* Message = NewObject<UROSMsgTwistWithCovariance>();
	Message->Twist = NewObject<UROSMsgTwist>();
	Message->Covariance = Covariance;
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgTwistWithCovariance does not have 36 values, it has %d"), Covariance.Num());
	return Message;
}

UROSMsgTwistWithCovariance* UROSMsgTwistWithCovariance::CreateEmpty()
{
	UROSMsgTwistWithCovariance* Message = NewObject<UROSMsgTwistWithCovariance>();
	Message->Twist = NewObject<UROSMsgTwist>();
	Message->Covariance = TArray<double>();
	Message->Covariance.SetNumZeroed(36);
	return Message;
}

void UROSMsgTwistWithCovariance::ToData(ROSData& Message) const
{
	ROSData SubElementTwist;
	Twist->ToData(SubElementTwist);
	DataHelpers::AppendSubDocument(Message,  "twist", SubElementTwist);
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Covariance Matrix in UROSMsgTwistWithCovariance does not have 36 values, it has %d"), Covariance.Num());

	DataHelpers::AppendTArray<double>(Message, "covariance", Covariance, [](ROSData& Array, const char* Key, const double& TArrayValue)
	{
		DataHelpers::AppendDouble(Array, Key, TArrayValue);
	});
}

bool UROSMsgTwistWithCovariance::FromData(const ROSData& Message)
{
	if(!Twist) Twist = NewObject<UROSMsgTwist>(this);
	
	ROSData SubElementTwist;
	return DataHelpers::ExtractSubDocument(Message, "twist", SubElementTwist) && Twist->FromData(SubElementTwist)
	&& DataHelpers::ExtractTArray<double>(Message, "covariance", Covariance, [](const ROSData& Array, const char* Key, double& Result)
	{
		return DataHelpers::ExtractDouble(Array, Key, Result);
	});
}
