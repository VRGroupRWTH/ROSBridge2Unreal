// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgPoseStamped.h"
#include "DataHelpers.h"

UROSMsgPoseStamped* UROSMsgPoseStamped::Create(UROSMsgHeader* Header, UROSMsgPose* Pose)
{
	UROSMsgPoseStamped* Message = NewObject<UROSMsgPoseStamped>();
	Message->Header = Header;
	Message->Pose = Pose;
	return Message;
}

UROSMsgPoseStamped* UROSMsgPoseStamped::CreateEmpty()
{
	UROSMsgPoseStamped* Message = NewObject<UROSMsgPoseStamped>();
	Message->Header = NewObject<UROSMsgHeader>();
	Message->Pose = NewObject<UROSMsgPose>();
	return Message;
}

void UROSMsgPoseStamped::ToData(ROSData& Message) const
{
	ROSData SubElementHeader;
	ROSData SubElementPose;
	Header->ToData(SubElementHeader);
	Pose->ToData(SubElementPose);
	DataHelpers::AppendSubDocument(Message,  "header", SubElementHeader);
	DataHelpers::AppendSubDocument(Message,  "pose", SubElementPose);
}

bool UROSMsgPoseStamped::FromData(const ROSData& Message)
{
	if(!Header) Header = NewObject<UROSMsgHeader>(this);
	if(!Pose) Pose = NewObject<UROSMsgPose>(this);
	
	ROSData SubElementHeader;
	ROSData SubElementPose;
	return DataHelpers::ExtractSubDocument(Message, "header", SubElementHeader) && Header->FromData(SubElementHeader)
	&& DataHelpers::ExtractSubDocument(Message, "pose", SubElementPose) && Pose->FromData(SubElementPose);
}
