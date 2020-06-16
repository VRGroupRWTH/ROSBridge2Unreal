// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgTransformStamped.h"
#include "DataHelpers.h"

UROSMsgTransformStamped* UROSMsgTransformStamped::Create(UROSMsgHeader* Header, const FString& ChildFrameID, UROSMsgTransform* Transform)
{
	UROSMsgTransformStamped* Message = NewObject<UROSMsgTransformStamped>();
	Message->Header = Header;
	Message->ChildFrameID = ChildFrameID;
	Message->Transform = Transform;
	return Message;
}

UROSMsgTransformStamped* UROSMsgTransformStamped::CreateEmpty()
{
	UROSMsgTransformStamped* Message = NewObject<UROSMsgTransformStamped>();
	Message->Header = NewObject<UROSMsgHeader>();
	Message->Transform = NewObject<UROSMsgTransform>();
	Message->ChildFrameID = "";
	return Message;
}

void UROSMsgTransformStamped::ToData(ROSData& Message) const
{
	ROSData SubElementHeader;
	ROSData SubElementTransform;
	Header->ToData(SubElementHeader);
	Transform->ToData(SubElementTransform);
	DataHelpers::AppendSubDocument(Message,  "header", SubElementHeader);
	DataHelpers::AppendString(Message,  "child_frame_id", ChildFrameID);
	DataHelpers::AppendSubDocument(Message,  "transform", SubElementTransform);
}

bool UROSMsgTransformStamped::FromData(const ROSData& Message)
{
	if(!Header) Header = NewObject<UROSMsgHeader>(this);
	if(!Transform) Transform = NewObject<UROSMsgTransform>(this);
	
	ROSData SubElementHeader;
	ROSData SubElementTransform;
	return DataHelpers::ExtractSubDocument(Message, "header", SubElementHeader) && Header->FromData(SubElementHeader)
	&& DataHelpers::ExtractString(Message, "child_frame_id", ChildFrameID)
	&& DataHelpers::ExtractSubDocument(Message, "transform", SubElementTransform) && Transform->FromData(SubElementTransform);
}
