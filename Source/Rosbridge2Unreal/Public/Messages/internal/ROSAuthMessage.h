#pragma once

#include "CoreMinimal.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "ROSAuthMessage.generated.h"

UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSAuthMessage : public UROSBridgeMessage
{
	GENERATED_BODY()

public:
	/* Data */
	FString Secret;
	FString Client;
	FString Destination;
	FString Level = "admin";

	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;
	
private:
	static FString EncodeSHA512(const FString Input);
};
