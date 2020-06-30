#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSTopicAdvertiseMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSTopicAdvertiseMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Data */
	FString MessageType;
	FString TopicName;
	FString ID;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
