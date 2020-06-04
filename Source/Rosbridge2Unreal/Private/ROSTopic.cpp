// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSTopic.h"

#include "LogCategory.h"
#include "ROSMessageBase.h"
#include "DataHelpers.h"

#include "IRosbridge2Unreal.h"
#include "ROSTopicAdvertiseMessage.h"
#include "ROSTopicPublishMessage.h"
#include "ROSTopicSubscribeMessage.h"
#include "ROSTopicUnadvertiseMessage.h"
#include "ROSTopicUnsubscribeMessage.h"

UROSTopic::UROSTopic(){}

void UROSTopic::Initialize(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass)
{
	StoredTopicName = TopicName;
	StoredMessageClass = MessageClass;
}

bool UROSTopic::Subscribe(TFunction<void(const UROSMessageBase* )>& Callback, const uint32 UniqueId)
{
	StoredCallbacks.Add(UniqueId, Callback);

	if(!IsSubscribed){
		UROSTopicSubscribeMessage* SubscribeMessage = NewObject<UROSTopicSubscribeMessage>();
		SubscribeMessage->ID = FString::Printf(TEXT("subscribe:%s"), *StoredTopicName);
		SubscribeMessage->TopicName = StoredTopicName;
		SubscribeMessage->MessageType = StoredMessageClass.GetDefaultObject()->GetMessageType();
		
		IsSubscribed = IRosbridge2Unreal::Get().SendMessage(*SubscribeMessage);
	}
	return IsSubscribed;
}

void UROSTopic::Unsubscribe(const uint32 UniqueId)
{
	StoredCallbacks.Remove(UniqueId);

	if(IsSubscribed && StoredCallbacks.Num() <= 0)
	{
		ForceUnsubscribeInternal();
	}
}

void UROSTopic::ForceUnsubscribeInternal()
{
	UROSTopicUnsubscribeMessage* UnsubscribeMessage = NewObject<UROSTopicUnsubscribeMessage>();
	UnsubscribeMessage->ID = FString::Printf(TEXT("subscribe:%s"), *StoredTopicName); // same ID as subscribe
	UnsubscribeMessage->TopicName = StoredTopicName;
	
	IsSubscribed = !IRosbridge2Unreal::Get().SendMessage(*UnsubscribeMessage);
}

void UROSTopic::IncomingMessage(const UROSTopicPublishMessage& Message)
{
	UROSMessageBase* ParsedMessage = NewObject<UROSMessageBase>(this, *StoredMessageClass);
	
	if(!ParsedMessage->FromData(Message.Data))
	{
		UE_LOG(LogROSBridge, Warning, TEXT("Error Parsing Message on Topic: %s"), *StoredTopicName);
		return;
	}
	
	Notify(ParsedMessage);
}

FString UROSTopic::GetTopicName() const
{
	return StoredTopicName;
}

bool UROSTopic::Advertise()
{
	if(!IsAdvertised){
		UROSTopicAdvertiseMessage* AdvertiseMessage = NewObject<UROSTopicAdvertiseMessage>();
		AdvertiseMessage->ID = FString::Printf(TEXT("advertise:%s"), *StoredTopicName);
		AdvertiseMessage->TopicName = StoredTopicName;
		AdvertiseMessage->MessageType = StoredMessageClass.GetDefaultObject()->GetMessageType();
		
		IsAdvertised = IRosbridge2Unreal::Get().SendMessage(*AdvertiseMessage);
	}

	return !IsAdvertised;
}

bool UROSTopic::Unadvertise()
{
	if(IsAdvertised){
		UROSTopicUnadvertiseMessage* UnadvertiseMessage = NewObject<UROSTopicUnadvertiseMessage>();
		UnadvertiseMessage->ID = FString::Printf(TEXT("advertise:%s"), *StoredTopicName);
		UnadvertiseMessage->TopicName = StoredTopicName;
		
		IsAdvertised = !IRosbridge2Unreal::Get().SendMessage(*UnadvertiseMessage);
	}

	return !IsAdvertised;
}

void UROSTopic::Publish(const UROSMessageBase* Message)
{
	if(!Message)
	{
		UE_LOG(LogROSBridge, Warning, TEXT("Tried to publish null message! Ignoring."));
		return;
	}
	
	Advertise(); //Ensure that this topic was advertised
	
	UROSTopicPublishMessage* PublishMessage = NewObject<UROSTopicPublishMessage>();
	
	Message->ToData(PublishMessage->Data);		

	PublishMessage->TopicName = StoredTopicName;
	PublishMessage->ID = FString::Printf(TEXT("publish:%s:%ld"), *StoredTopicName, IRosbridge2Unreal::Get().GetNextID());
	
	IRosbridge2Unreal::Get().SendMessage(*PublishMessage);
}

void UROSTopic::Notify(UROSMessageBase* Message)
{
	for(auto Callback : StoredCallbacks)
	{
		Callback.Value(Message);
	}
}
