// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgTwistWithCovarianceBP.h"
#include "DataHelpers.h"
#include "LogCategory.h"

UROSMsgTwistWithCovarianceBP* UROSMsgTwistWithCovarianceBP::Create(UROSMsgTwistBP* Twist, const TArray<float>& Covariance)
{
	UROSMsgTwistWithCovarianceBP* Message = NewObject<UROSMsgTwistWithCovarianceBP>();
	Message->Twist = NewObject<UROSMsgTwistBP>();
	Message->Covariance = Covariance;
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Given Covariance Matrix in UROSMsgTwistWithCovarianceBP does not have 36 values, it has %d"), Covariance.Num());
	return Message;
}

UROSMsgTwistWithCovarianceBP* UROSMsgTwistWithCovarianceBP::CreateEmpty()
{
	UROSMsgTwistWithCovarianceBP* Message = NewObject<UROSMsgTwistWithCovarianceBP>();
	Message->Twist = NewObject<UROSMsgTwistBP>();
	Message->Covariance = TArray<float>();
	Message->Covariance.SetNumZeroed(36);
	return Message;
}

void UROSMsgTwistWithCovarianceBP::ToData(ROSData& Message) const
{
	ROSData SubElementTwist;
	Twist->ToData(SubElementTwist);
	DataHelpers::AppendSubDocument(Message,  "twist", SubElementTwist);
	if(Covariance.Num() != 36) UE_LOG(LogROSBridge, Warning, TEXT("Covariance Matrix in UROSMsgTwistWithCovarianceBP does not have 36 values, it has %d"), Covariance.Num());

	DataHelpers::AppendTArray<float>(Message, "covariance", Covariance, [](ROSData& Array, const char* Key, const float& TArrayValue)
	{
		DataHelpers::AppendFloat(Array, Key, TArrayValue);
	});
}

bool UROSMsgTwistWithCovarianceBP::FromData(const ROSData& Message)
{
	if(!Twist) Twist = NewObject<UROSMsgTwistBP>(this);
	
	ROSData SubElementTwist;
	return DataHelpers::ExtractSubDocument(Message, "twist", SubElementTwist) && Twist->FromData(SubElementTwist)
	&& DataHelpers::ExtractTArray<float>(Message, "covariance", Covariance, [](const ROSData& Array, const char* Key, float& Result)
	{
		return DataHelpers::ExtractFloat(Array, Key, Result);
	});
}
