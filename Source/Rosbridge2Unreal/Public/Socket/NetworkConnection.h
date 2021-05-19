#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"

#include "NetworkConnection.generated.h"

UCLASS(Abstract)
class ROSBRIDGE2UNREAL_API UNetworkConnection : public UObject {
	GENERATED_BODY()

protected:
	ETransportMode CurrentTransportMode = ETransportMode::JSON;
	TFunction<void(ROSData)> IncomingMessageCallback;
	
public:
	virtual bool Initialize(FString IPAddress, int Port, ETransportMode Mode) PURE_VIRTUAL(UNetworkConnection::Initialize, return true;);
	virtual void Uninitialize() PURE_VIRTUAL(UNetworkConnection::Uninitialize, return;);
	virtual bool SendMessage(const uint8_t *Data, unsigned int Length) const PURE_VIRTUAL(UNetworkConnection::SendMessage, return true;);
	
	bool SendMessage(const ROSData& Data) const;
	
	inline bool SendMessage(const FString Data) const
	{
		return SendMessage(reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*Data)), Data.Len());
	}

	inline void RegisterIncomingMessageCallback(TFunction<void(ROSData)> CallbackFunction)
	{
		IncomingMessageCallback = CallbackFunction;
	}
	
	void ReportError(ETransportError Error) const;

	inline void SetTransportMode(ETransportMode Mode)
	{
		CurrentTransportMode = Mode;
	}

	inline ETransportMode GetTransportMode() const
	{
		return CurrentTransportMode;
	}
};
