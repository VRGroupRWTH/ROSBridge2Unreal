#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Messages/ROSMessageBase.h"

#include "Messages/internal/ROSTopicPublishMessage.h"
#include "ROSTopic.generated.h"


/**
 * Class that is internally used for the communication with the ROS Bridge
 * Users should have a look at the UROSTopicHandle
 * UROSTopicHandles proxy this object such that many components can listen to the same topic
 */
UCLASS()
class UROSTopic : public UObject
{
	GENERATED_BODY()
	
public:	
	
	/**
	 * Initializes this topic instance
	 * @param TopicName - Topic name that is used to subscribe/advertize later. Used as an ID internally.
	 * @param MessageClass - The message class that is used to (de-)serialize
	 */
	void Initialize(const FString& TopicName, TSubclassOf<UROSMessageBase> MessageClass);

	/**
	 * Send subscribe message to the ROS Bridge and store a callback that is used for notification in the event of a new message
	 * @param Callback - The callback that is called in the event of a new message
	 * @param UniqueId - UniqueId that is stored together with the callback to reidentify it.
	 * @param InReusableMessage - Message class instance that is used for deserialization (shared between all handles)
	 * @return Success of subscription
	 */
	bool Subscribe(TFunction<void(const UROSMessageBase*)>& Callback, uint32 UniqueId, UROSMessageBase* InReusableMessage = nullptr);
	
	/**
	 * Unsubscribe the callback associated with the given ID
	 * @param UniqueId - ID that was given on Subscribe call
	 */
	void Unsubscribe(uint32 UniqueId);
	
	/**
	 * Force the sending of an unsubscribe message to the ROS Bridge
	 * @return Success of sending
	 */
	bool ForceUnsubscribeInternal();

	/**
	 * Send an advertise message to the ROS Bridge
	 * @return Success of sending
	 */
	bool Advertise();
	
	/**
	 * Send an unadverise message to the ROS Bridge
	 * @return Success of sending
	 */
	bool Unadvertise();
	
	/**
	 * Publish a message on the topic to the ROS Bridge
	 * @param Message - The message to send
	 */
	void Publish(const UROSMessageBase* Message);
	
	/**
	 * Internally used to process a raw message and distribute to the handles
	 * @param Message - The message to process
	 */
	void IncomingMessage(const UROSTopicPublishMessage& Message);

	/**
	 * @return The topic name specified in the initialize call. Internally used as an ID
	 */
	FString GetTopicName() const;
	
private:

	UPROPERTY() UROSMessageBase* ReusableMessage = nullptr;
	
	bool IsSubscribed = false;
	bool IsAdvertised = false;
	TMap<uint32,TFunction<void(const UROSMessageBase*)>> StoredCallbacks;
	FString StoredTopicName = "";
	TSubclassOf<UROSMessageBase> StoredMessageClass;
	void Notify(UROSMessageBase* Message);
};