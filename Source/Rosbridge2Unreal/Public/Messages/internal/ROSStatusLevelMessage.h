#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSStatusLevelMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSStatusLevelMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Data */
	FString ID;
	EStatusMessageLevel Level = EStatusMessageLevel::Error;

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
};
