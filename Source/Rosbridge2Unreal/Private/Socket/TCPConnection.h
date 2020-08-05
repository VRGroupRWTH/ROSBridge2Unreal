#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "TypeDefinitions.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

#include "TCPConnection.generated.h"

UCLASS()
class UTCPConnection : public UObject, public FRunnable {
	GENERATED_BODY()
	
public:
	bool Initialize(FString IPAddress, int Port, ETransportMode Mode);
	void Uninitialize();
	bool SendMessage(ROSData& Data) const;
	bool SendMessage(FString Data) const;
	bool SendMessage(const uint8_t *Data, unsigned int Length) const;
	void RegisterIncomingMessageCallback(TFunction<void(ROSData)> CallbackFunction);
	void ReportError(ETransportError Error) const;
	void SetTransportMode(ETransportMode);

	bool IsHealthy() const;
	ETransportMode GetTransportMode() const;
	
private:
	FSocket *Socket = nullptr;
	ETransportMode CurrentTransportMode = ETransportMode::JSON;

	FRunnableThread* ReceiverThread = nullptr;
	uint32 Run() override;
	bool bReceiverThreadRunning = true;
	bool bTerminateReceiverThread = false;
	
	TFunction<void(ROSData)> IncomingMessageCallback;
};