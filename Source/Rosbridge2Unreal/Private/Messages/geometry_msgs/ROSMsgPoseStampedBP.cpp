// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/geometry_msgs/ROSMsgPoseStampedBP.h"
#include "DataHelpers.h"

UROSMsgPoseStampedBP* UROSMsgPoseStampedBP::Create(UROSMsgHeader* Header, UROSMsgPoseBP* Pose)
{
	UROSMsgPoseStampedBP* Message = NewObject<UROSMsgPoseStampedBP>();
	Message->Header = Header;
	Message->Pose = Pose;
	return Message;
}

UROSMsgPoseStampedBP* UROSMsgPoseStampedBP::CreateEmpty()
{
	UROSMsgPoseStampedBP* Message = NewObject<UROSMsgPoseStampedBP>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Pose = NewObject<UROSMsgPoseBP>(Message);
	return Message;
}

void UROSMsgPoseStampedBP::ToData(ROSData& Message) const
{
	ROSData SubElementHeader;
	ROSData SubElementPose;
	Header->ToData(SubElementHeader);
	Pose->ToData(SubElementPose);
	DataHelpers::AppendSubDocument(Message,  "header", SubElementHeader);
	DataHelpers::AppendSubDocument(Message,  "pose", SubElementPose);
}

bool UROSMsgPoseStampedBP::FromData(const ROSData& Message)
{
	if(!Header) Header = NewObject<UROSMsgHeader>(this);
	if(!Pose) Pose = NewObject<UROSMsgPoseBP>(this);
	
	ROSData SubElementHeader;
	ROSData SubElementPose;
	return DataHelpers::ExtractSubDocument(Message, "header", SubElementHeader) && Header->FromData(SubElementHeader)
	&& DataHelpers::ExtractSubDocument(Message, "pose", SubElementPose) && Pose->FromData(SubElementPose);
}
