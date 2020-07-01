#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Messages/ROSMessageBase.h"

#include "Messages/internal/ROSTopicPublishMessage.h"
#include "ROSTopic.generated.h"


UCLASS()
class UROSTopic : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UROSTopic();

	void Initialize(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass);

	bool Subscribe(TFunction<void(const UROSMessageBase*)>& Callback, uint32 UniqueId, UROSMessageBase* InReusableMessage = nullptr);
	void Unsubscribe(uint32 UniqueId);
	void ForceUnsubscribeInternal();

	bool Advertise();
	bool Unadvertise();
	void Publish(const UROSMessageBase* Message);
	
	void IncomingMessage(const UROSTopicPublishMessage& Message);

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