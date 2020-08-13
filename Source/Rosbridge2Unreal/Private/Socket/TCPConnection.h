#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "TypeDefinitions.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "NetworkConnection.h"

#include "TCPConnection.generated.h"

UCLASS()
class ROSBRIDGE2UNREAL_API UTCPConnection : public UNetworkConnection, public FRunnable {
	GENERATED_BODY()
	
public:
	bool Initialize(FString IPAddress, int Port, ETransportMode Mode) override;
	void Uninitialize() override;
	bool SendMessage(const uint8_t *Data, unsigned int Length) const override;
	
private:
	FSocket *Socket = nullptr;

	FRunnableThread* ReceiverThread = nullptr;
	uint32 Run() override;
	bool bReceiverThreadRunning = true;
	bool bTerminateReceiverThread = false;
};