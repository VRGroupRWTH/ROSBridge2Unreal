#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"
#include "Socket/NetworkConnection.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"

#include "WebSockConnection.generated.h"

UCLASS()
class ROSBRIDGE2UNREAL_API UWebSockConnection : public UNetworkConnection {
	GENERATED_BODY()
	
public:
	bool Initialize(FString IPAddress, int Port, ETransportMode Mode) override;
	void Uninitialize() override;
	bool SendMessage(const uint8_t *Data, unsigned int Length) const override;
	
private:
	TSharedPtr<IWebSocket> Socket = nullptr;
	TArray<uint8> BinaryBuffer;
	uint64 CurrentMessageLength = 0;

	void Receive(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
};