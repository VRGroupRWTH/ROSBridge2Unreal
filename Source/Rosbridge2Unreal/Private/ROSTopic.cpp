#include "ROSTopic.h"

#include "LogCategory.h"
#include "Messages/ROSMessageBase.h"

#include "IRosbridge2Unreal.h"
#include "Messages/internal/ROSTopicAdvertiseMessage.h"
#include "Messages/internal/ROSTopicPublishMessage.h"
#include "Messages/internal/ROSTopicSubscribeMessage.h"
#include "Messages/internal/ROSTopicUnadvertiseMessage.h"
#include "Messages/internal/ROSTopicUnsubscribeMessage.h"

void UROSTopic::Initialize(const FString& TopicName, TSubclassOf<UROSMessageBase> MessageClass)
{
	InitializeConnection();

	if (!MessageClass)
	{
		UE_LOG(LogROSBridge, Error, TEXT("No class given for initialization of ROSTopic for TopicName %s."), *TopicName);
		return;
	}

	StoredTopicName = TopicName;
	StoredMessageClass = MessageClass;
}

bool UROSTopic::Subscribe(const TFunction<void(const UROSMessageBase*)>& Callback, UROSMessageBase* InReusableMessage)
{
	if (!bInitialized) {
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSTopic before you Subscribe to it."));
		return false;
	}

	StoredCallback = Callback;
	ReusableMessage = InReusableMessage;

	if (!IsSubscribed) {
		UROSTopicSubscribeMessage* SubscribeMessage = NewObject<UROSTopicSubscribeMessage>();
		SubscribeMessage->ID = FString::Printf(TEXT("subscribe:%s"), *StoredTopicName);
		SubscribeMessage->TopicName = StoredTopicName;
		SubscribeMessage->MessageType = StoredMessageClass.GetDefaultObject()->GetMessageType();
		if (IRosbridge2Unreal::Get().GetSettings()->bFragmentMessages)
		{
			SubscribeMessage->FragmentSize = IRosbridge2Unreal::Get().GetSettings()->FragmentSize;
		}

		IsSubscribed = SendMessage(*SubscribeMessage);
	}
	return IsSubscribed;
}

void UROSTopic::Unsubscribe()
{
	UROSTopicUnsubscribeMessage* UnsubscribeMessage = NewObject<UROSTopicUnsubscribeMessage>();

	UnsubscribeMessage->ID = FString::Printf(TEXT("subscribe:%s"), *StoredTopicName); // same ID as subscribe
	UnsubscribeMessage->TopicName = StoredTopicName;

	IsSubscribed = !SendMessage(*UnsubscribeMessage);
}

void UROSTopic::IncomingMessage(const UROSTopicPublishMessage& Message)
{
	UROSMessageBase* ParsedMessage = ReusableMessage;

	if (!ParsedMessage) //Only generate new, if we don't have a reusable message
	{
		ParsedMessage = NewObject<UROSMessageBase>(this, *StoredMessageClass);
	}

	if (!ParsedMessage->FromData(Message.Data))
	{
		UE_LOG(LogROSBridge, Warning, TEXT("Error Parsing Message on Topic: %s"), *StoredTopicName);
		return;
	}

	if (StoredCallback) StoredCallback(ParsedMessage);
}

FString UROSTopic::GetTopicName() const
{
	return StoredTopicName;
}

bool UROSTopic::HandleMessage(const FString& OPCode, const ROSData& Message)
{
	if (OPCode == "publish") //Message for Topic
	{
		UROSTopicPublishMessage* TopicMessage = NewObject<UROSTopicPublishMessage>();
		if (!TopicMessage->FromData(Message)) return false;
		if (GetTopicName() == TopicMessage->TopicName)
		{
			IncomingMessage(*TopicMessage);
			return true;
		}

		UE_LOG(LogROSBridge, Verbose, TEXT("Received message for a topic we dont know."));
		return false;
	}
	return false;
}

bool UROSTopic::Advertise()
{
	if (!bInitialized) {
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSTopic before you Advertise it."));
		return false;
	}

	if (!IsAdvertised) {
		UROSTopicAdvertiseMessage* AdvertiseMessage = NewObject<UROSTopicAdvertiseMessage>();
		AdvertiseMessage->ID = FString::Printf(TEXT("advertise:%s"), *StoredTopicName);
		AdvertiseMessage->TopicName = StoredTopicName;
		AdvertiseMessage->MessageType = StoredMessageClass.GetDefaultObject()->GetMessageType();

		IsAdvertised = SendMessage(*AdvertiseMessage);
	}

	return !IsAdvertised;
}

bool UROSTopic::Unadvertise()
{
	if (!bInitialized) {
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSTopic before you Unadversize it."));
		return false;
	}

	if (IsAdvertised) {
		UROSTopicUnadvertiseMessage* UnadvertiseMessage = NewObject<UROSTopicUnadvertiseMessage>();
		UnadvertiseMessage->ID = FString::Printf(TEXT("advertise:%s"), *StoredTopicName);
		UnadvertiseMessage->TopicName = StoredTopicName;

		IsAdvertised = !SendMessage(*UnadvertiseMessage);
	}

	return !IsAdvertised;
}

void UROSTopic::Publish(const UROSMessageBase* Message)
{
	if (!bInitialized) {
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSTopic before you Publish to it."));
		return;
	}

	if (!Message)
	{
		UE_LOG(LogROSBridge, Warning, TEXT("Tried to publish null message! Ignoring."));
		return;
	}

	Advertise(); //Ensure that this topic was advertised

	UROSTopicPublishMessage* PublishMessage = NewObject<UROSTopicPublishMessage>();

	Message->ToData(PublishMessage->Data);

	PublishMessage->TopicName = StoredTopicName;
	PublishMessage->ID = FString::Printf(TEXT("publish:%s:%ld"), *StoredTopicName, GetNextID());

	SendMessage(*PublishMessage);
}

void UROSTopic::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	switch (EndPlayReason)
	{
	case EEndPlayReason::LevelTransition:
	case EEndPlayReason::EndPlayInEditor:
	case EEndPlayReason::RemovedFromWorld:
	case EEndPlayReason::Quit:
		Unsubscribe();
		UninitializeConnection();
	case EEndPlayReason::Destroyed: break;
	}
}

void UROSTopic::Subscribe()
{
	Subscribe([this](const UROSMessageBase* Message) {OnNewMessage.Broadcast(Message); });
}

void UROSTopic::SubscribeWithReusableMessage(UROSMessageBase* InReusableMessage)
{
	Subscribe([this](const UROSMessageBase* Message) {OnNewMessage.Broadcast(Message); }, InReusableMessage);
}