#pragma once
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Engine/DeveloperSettings.h"
#include "Socket/TCPConnection.h"
#include "RosbridgeSettings.generated.h"

UCLASS(config=Game, defaultconfig, meta=(DisplayName="ROS Bridge"))
class URosbridgeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Bridge IP Address"))
	FString IP = "127.0.0.1";

	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Bridge Port"))
	int Port = 9090;

	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Transportation Mode"))
	TransportMode TransportationMode = TransportMode::JSON;

	UPROPERTY(EditAnywhere, config, Category = "Connection", meta = (DisplayName = "Simulate Connection"))
	bool bSimulateConnection = false;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "Emit Clock Events from Unreal"))
	bool bEmitClockEvents = false;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "Use Wall Clock Time, else uses the Gameplay Time"))
	bool bUseWallClockTime = true;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "Use Fixed Update Interval"))
	bool bUseFixedUpdateInterval = false;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "The Used Update Interval", ClampMin="0"))
	double FixedUpdateInterval = true;
};
