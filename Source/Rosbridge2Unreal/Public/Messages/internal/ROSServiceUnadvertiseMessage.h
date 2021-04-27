#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSServiceUnadvertiseMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSServiceUnadvertiseMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Request Data */
	FString ServiceName;
	FString ID;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
