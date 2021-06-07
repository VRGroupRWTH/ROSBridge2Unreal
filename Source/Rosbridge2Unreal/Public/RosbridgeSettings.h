#pragma once
#include "CoreMinimal.h"
#include "TypeDefinitions.h"
#include "Engine/DeveloperSettings.h"
#include "RosbridgeSettings.generated.h"

/**
 * Settings class that creates a settings page in the project settings
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="ROS Bridge"))
class ROSBRIDGE2UNREAL_API URosbridgeSettings : public UDeveloperSettings
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

	UPROPERTY(EditAnywhere, config, Category = "Authentication")
	bool bShouldAuthenticate = false;

	UPROPERTY(EditAnywhere, config, Category = "Authentication", meta = (EditCondition = "bShouldAuthenticate"))
	FString Secret = "";

	UPROPERTY(EditAnywhere, config, Category = "Fragmentation")
	bool bFragmentMessages = false;

	UPROPERTY(EditAnywhere, config, Category = "Fragmentation", meta = (EditCondition = "bFragmentMessages"))
	int FragmentSize = 60000;
};
