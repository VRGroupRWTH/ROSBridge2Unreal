// Fill out your copyright notice in the Description page of Project Settings.


#include "Messages/geometry_msgs/ROSMsgTransformStampedBP.h"
#include "DataHelpers.h"

UROSMsgTransformStampedBP* UROSMsgTransformStampedBP::Create(UROSMsgHeader* Header, const FString& ChildFrameID, UROSMsgTransformBP* Transform)
{
	UROSMsgTransformStampedBP* Message = NewObject<UROSMsgTransformStampedBP>();
	Message->Header = Header;
	Message->ChildFrameID = ChildFrameID;
	Message->Transform = Transform;
	return Message;
}

UROSMsgTransformStampedBP* UROSMsgTransformStampedBP::CreateEmpty()
{
	UROSMsgTransformStampedBP* Message = NewObject<UROSMsgTransformStampedBP>();
	Message->Header = NewObject<UROSMsgHeader>();
	Message->Transform = NewObject<UROSMsgTransformBP>();
	Message->ChildFrameID = "";
	return Message;
}

void UROSMsgTransformStampedBP::ToData(ROSData& Message) const
{
	ROSData SubElementHeader;
	ROSData SubElementTransform;
	Header->ToData(SubElementHeader);
	Transform->ToData(SubElementTransform);
	DataHelpers::AppendSubDocument(Message,  "header", SubElementHeader);
	DataHelpers::AppendString(Message,  "child_frame_id", ChildFrameID);
	DataHelpers::AppendSubDocument(Message,  "transform", SubElementTransform);
}

bool UROSMsgTransformStampedBP::FromData(const ROSData& Message)
{
	if(!Header) Header = NewObject<UROSMsgHeader>(this);
	if(!Transform) Transform = NewObject<UROSMsgTransformBP>(this);
	
	ROSData SubElementHeader;
	ROSData SubElementTransform;
	return DataHelpers::ExtractSubDocument(Message, "header", SubElementHeader) && Header->FromData(SubElementHeader)
	&& DataHelpers::ExtractString(Message, "child_frame_id", ChildFrameID)
	&& DataHelpers::ExtractSubDocument(Message, "transform", SubElementTransform) && Transform->FromData(SubElementTransform);
}
