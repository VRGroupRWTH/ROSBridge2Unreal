#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSTopicSubscribeMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSTopicSubscribeMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:	
	/* Data */
	FString ID;
	FString TopicName;
	FString MessageType;
	FString Compression = "none";
	int ThrottleRate = 0;
	int QueueLength = 10;
	int FragmentSize = -1;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
