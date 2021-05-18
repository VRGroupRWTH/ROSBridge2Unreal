#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Messages/ROSMessageBase.h"

#include "Messages/internal/ROSTopicPublishMessage.h"
#include "ROSBridgeConnection.h"
#include "ROSTopic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveSignature, const UROSMessageBase*, Message);

/**
 * This component can be added to an actor to receive/send message to the ROS Bridge
 */
UCLASS( ClassGroup=(ROS), meta=(BlueprintSpawnableComponent), Blueprintable )
class UROSTopic : public UROSBridgeConnection
{
	GENERATED_BODY()
	
public:	
	
	/**
	 * Initializes this topic
	 * @param TopicName - Topic name that is used to subscribe/advertize later. Used as an ID internally.
	 * @param MessageClass - The message class that is used to (de-)serialize
	 */
	UFUNCTION(BlueprintCallable) void Initialize(const FString& TopicName, TSubclassOf<UROSMessageBase> MessageClass);

	/**
	 * Subscribe this topic and get notified if a new message arrives (C++ variant)
	 * @param Callback - Function that is called if a new message arrives
	 * @param InReusableMessage - Message instance that is reused for deserialization (optional, shared between all handles on the same topic)
	 */
	bool Subscribe(const TFunction<void(const UROSMessageBase*)>& Callback, UROSMessageBase* InReusableMessage = nullptr);
	
	/**
	 * Unsubscribe this topic. Automatically called on exit.
	 */
	UFUNCTION(BlueprintCallable) void Unsubscribe();

	/**
	 * Send an advertise message to the ROS Bridge. Called automatically on publish.
	 * @return Success of sending
	 */
	UFUNCTION(BlueprintCallable) bool Advertise();
	
	/**
	 * Send an unadverise message to the ROS Bridge
	 * @return Success of sending
	 */
	UFUNCTION(BlueprintCallable) bool Unadvertise();
	
	/**
	 * Publish a message on the topic to the ROS Bridge
	 * @param Message - The message to send
	 */
	UFUNCTION(BlueprintCallable) void Publish(const UROSMessageBase* Message);
	
	/**
	 * @return The topic name specified in the initialize call. Internally used as an ID
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetTopicName() const;


	/**
	 * Event that is called if a new message arrives (Blueprint variant)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintAssignable) FReceiveSignature OnNewMessage;

	/*
	 * Unsubscribes and kills connection on Endplay
	 */
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	/**
	 * Subscribe this handle to the topic and get notified on the `OnNewMessage` event if a new message arrives (Blueprint variant)
	 */
	UFUNCTION(BlueprintCallable) void Subscribe();
	
	/**
	 * Like subscribe, but with a reusable message
	 * @param ReusableMessage - Message instance that is reused for deserialization
	 */
	UFUNCTION(BlueprintCallable) void SubscribeWithReusableMessage(UROSMessageBase* ReusableMessage);
	
	/**
	 * Internally used to process a raw message and distribute to the handles
	 * @param Message - The message to process
	 */
	void IncomingMessage(const UROSTopicPublishMessage& Message);

	/*
	 * Handle all message with OPCode "publish"
	 */
	bool HandleMessage(const FString& OPCode, const ROSData& Message) override;
	
	UPROPERTY() UROSMessageBase* ReusableMessage = nullptr;
	
	bool IsSubscribed = false;
	bool IsAdvertised = false;
	TFunction<void(const UROSMessageBase*)> StoredCallback = nullptr;
	FString StoredTopicName = "";
	TSubclassOf<UROSMessageBase> StoredMessageClass;
};