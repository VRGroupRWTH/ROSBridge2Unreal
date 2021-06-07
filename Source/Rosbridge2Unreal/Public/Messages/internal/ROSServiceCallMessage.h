#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSServiceCallMessage.generated.h"

UCLASS()
class ROSBRIDGE2UNREAL_API UROSServiceCallMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Data */
	ROSData Data;

	/* Request Data */
	FString ID;
	FString ServiceName;
	int FragmentSize = -1;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
