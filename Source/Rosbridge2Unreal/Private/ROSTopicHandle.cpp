#include "ROSTopicHandle.h"
#include "IRosbridge2Unreal.h"
#include "LogCategory.h"

// Sets default values for this component's properties
UROSTopicHandle::UROSTopicHandle(){}

void UROSTopicHandle::Initialize(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass)
{
	TopicHandle = IRosbridge2Unreal::Get().GetTopic(TopicName, MessageClass);
}

void UROSTopicHandle::Subscribe(TFunction<void(const UROSMessageBase*)> Callback, UROSMessageBase* ReusableMessage) const
{
	if(!TopicHandle){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSTopicHandle before you Subscribe to it."));
		return;
	}
	TopicHandle->Subscribe(Callback, GetUniqueID(), ReusableMessage);
}

void UROSTopicHandle::Subscribe() const
{
	Subscribe([this](const UROSMessageBase* Message){OnNewMessage.Broadcast(Message);});
}

void UROSTopicHandle::SubscribeWithReusableMessage(UROSMessageBase* ReusableMessage) const
{
	Subscribe([this](const UROSMessageBase* Message){OnNewMessage.Broadcast(Message);}, ReusableMessage);
}

void UROSTopicHandle::Publish(const UROSMessageBase* Message) const
{
	if(!TopicHandle){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSTopicHandle before you Publish on it."));
		return;
	}
	TopicHandle->Publish(Message);
}

void UROSTopicHandle::Unsubscribe() const
{
	if(!TopicHandle){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSTopicHandle before you Unsubscribe from it."));
		return;
	}
	TopicHandle->Unsubscribe(GetUniqueID());
}

void UROSTopicHandle::Unadvertise() const
{
	if(!TopicHandle){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSTopicHandle before you Unadvertise it."));
		return;
	}
	TopicHandle->Unadvertise();
}
