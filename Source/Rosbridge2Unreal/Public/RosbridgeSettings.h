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
	bool SimulateConnection = false;
};
