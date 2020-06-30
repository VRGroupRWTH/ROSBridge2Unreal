#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Messages/ROSMessageBase.h"
#include "ROSTopic.h"
#include "ROSTopicHandle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveSignature, const UROSMessageBase*, Message);

UCLASS( ClassGroup=(ROS), meta=(BlueprintSpawnableComponent), Blueprintable )
class ROSBRIDGE2UNREAL_API UROSTopicHandle : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UROSTopicHandle();

	UFUNCTION(BlueprintCallable) void Initialize(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass);
	void Subscribe(TFunction<void(const UROSMessageBase*)> Callback) const; /* C++ only */
	UFUNCTION(BlueprintCallable) void Publish(const UROSMessageBase* Message) const;
	UFUNCTION(BlueprintCallable) void Unsubscribe() const;
	UFUNCTION(BlueprintCallable) void Unadvertise() const;
	
private:
	UPROPERTY() UROSTopic* TopicHandle = nullptr;
	UFUNCTION(BlueprintCallable) void Subscribe() const; /* Blueprint only */
	UPROPERTY(VisibleAnywhere, BlueprintAssignable) FReceiveSignature OnNewMessage; /* Blueprint only */
};
