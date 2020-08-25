#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Messages/ROSMessageBase.h"
#include "ROSTopic.h"
#include "ROSTopicHandle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveSignature, const UROSMessageBase*, Message);

/**
 * This component can be added to an actor to receive/send message to the ROS Bridge
 * It serves as a proxy to the internally uses ROSTopic
 */
UCLASS( ClassGroup=(ROS), meta=(BlueprintSpawnableComponent), Blueprintable )
class ROSBRIDGE2UNREAL_API UROSTopicHandle : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	
	/**
	 * Initialize this topic handle
	 * @param TopicName - Topic name that is used to subscribe/advertise later. Used as an ID internally.
	 * @param MessageClass - The message class that is used to (de-)serialize
	 */
	UFUNCTION(BlueprintCallable) void Initialize(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass);

	/**
	 * Subscribe this handle to the topic and get notified if a new message arrives (C++ variant)
	 * @param Callback - Function that is called if a new message arrives
	 * @param ReusableMessage - Message instance that is reused for deserialization (optional, shared between all handles on the same topic)
	 */
	void Subscribe(TFunction<void(const UROSMessageBase*)> Callback, UROSMessageBase* ReusableMessage = nullptr) const;
	
	/**
	 * Publish a message to the topic. Automatically advertise if not done already.
	 * @param Message - Message to publish
	 */
	UFUNCTION(BlueprintCallable) void Publish(const UROSMessageBase* Message) const;
	
	/**
	 * Unsubscribe this handle from the topic. Automatically called on exit.
	 */
	UFUNCTION(BlueprintCallable) void Unsubscribe() const;
	
	/**
	 * Unadvertise this topic explicitly. Automatically called on exit.
	 */
	UFUNCTION(BlueprintCallable) void Unadvertise() const;
	
private:
	UPROPERTY() UROSTopic* TopicHandle = nullptr;
	
	/**
	 * Subscribe this handle to the topic and get notified on the `OnNewMessage` event if a new message arrives (Blueprint variant)
	 */
	UFUNCTION(BlueprintCallable) void Subscribe() const;
	
	/**
	 * Like subscribe, but with a reusable message
	 * @param ReusableMessage - Message instance that is reused for deserialization
	 */
	UFUNCTION(BlueprintCallable) void SubscribeWithReusableMessage(UROSMessageBase* ReusableMessage) const;
	
	/**
	 * Event that is called if a new message arrives (Blueprint variant)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintAssignable) FReceiveSignature OnNewMessage;
};
