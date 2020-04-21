#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "TypeDefinitions.h"
#include "Runnable.h"
#include "RunnableThread.h"

#include "TCPConnection.generated.h"

UCLASS()
class UTCPConnection : public UObject, public FRunnable {
	GENERATED_BODY()
	
public:
	bool Initialize(FString IPAddress, int Port, TransportMode Mode);
	void Uninitialize();
	bool SendMessage(ROSData& Data) const;
	bool SendMessage(FString Data) const;
	bool SendMessage(const uint8_t *Data, unsigned int Length) const;
	void RegisterIncomingMessageCallback(TFunction<void(ROSData)> CallbackFunction);
	void ReportError(TransportError Error) const;
	void SetTransportMode(TransportMode);

	bool IsHealthy() const;
	TransportMode GetTransportMode() const;
	
private:
	FSocket *Socket = nullptr;
	TransportMode CurrentTransportMode = TransportMode::JSON;

	FRunnableThread* ReceiverThread = nullptr;
	uint32 Run() override;
	bool bReceiverThreadRunning = true;
	bool bTerminateReceiverThread = false;
	
	TFunction<void(ROSData)> IncomingMessageCallback;
};