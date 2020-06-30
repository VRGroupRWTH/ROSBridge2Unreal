#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSTopicPublishMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSTopicPublishMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Data */
	ROSData Data;
	
	/* Request Data */
	FString ID;
	FString TopicName;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
