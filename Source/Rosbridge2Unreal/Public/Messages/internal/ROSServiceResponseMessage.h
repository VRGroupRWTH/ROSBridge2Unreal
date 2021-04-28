#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSServiceResponseMessage.generated.h"

UCLASS()
class ROSBRIDGE2UNREAL_API UROSServiceResponseMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Data */
	ROSData Data;

	/* Request Data */
	FString ID;
	FString ServiceName;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
