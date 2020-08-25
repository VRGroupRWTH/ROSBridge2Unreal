#pragma once
#include "CoreMinimal.h"
#include "TypeDefinitions.h"
#include "Engine/DeveloperSettings.h"
#include "RosbridgeSettings.generated.h"

/**
 * Settings class that creates a settings page in the project settings
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="ROS Bridge"))
class URosbridgeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Simulate Connection"))
	bool bSimulateConnection = false;
	
	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Bridge IP Address", EditCondition = "!bSimulateConnection"))
	FString IP = "127.0.0.1";

	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Bridge Port", EditCondition = "!bSimulateConnection"))
	int Port = 9090;

	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Transportation Mode"))
	ESocketMode SocketMode = ESocketMode::TCP;
	
	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Transportation Mode"))
	ETransportMode TransportationMode = ETransportMode::JSON;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "Emit Clock Events from Unreal"))
	bool bEmitClockEvents = false;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "Use Wall Clock Time, else uses the Gameplay Time", EditCondition = "bEmitClockEvents"))
	bool bUseWallClockTime = true;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "Use Fixed Update Interval", EditCondition = "bEmitClockEvents"))
	bool bUseFixedUpdateInterval = false;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "The Used Update Interval", ClampMin="0", EditCondition = "bEmitClockEvents && bUseFixedUpdateInterval"))
	double FixedUpdateInterval = 0.1;
};
