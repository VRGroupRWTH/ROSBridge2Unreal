#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"
#include "LogCategory.h"

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
	
	bool SendMessage(const ROSData& Data) const
	{
		if (CurrentTransportMode == ETransportMode::BSON)
		{
			std::vector<uint8> EncodedData;
			jsoncons::bson::encode_bson(Data,EncodedData);
			return SendMessage(EncodedData.data(), EncodedData.size());
		}
		else if (CurrentTransportMode == ETransportMode::JSON)
		{
			std::string EncodedData;
			Data.dump(EncodedData, jsoncons::indenting::no_indent);
			return SendMessage(reinterpret_cast<const uint8*>(EncodedData.c_str()), EncodedData.length());
		}
		return false;
	};
	
	bool SendMessage(const FString Data) const
	{
		return SendMessage(reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*Data)), Data.Len());
	}

	void RegisterIncomingMessageCallback(TFunction<void(ROSData)> CallbackFunction)
	{
		IncomingMessageCallback = CallbackFunction;
	}
	
	void ReportError(ETransportError Error) const
	{
		switch(Error)
		{
		case ETransportError::SocketError:
			UE_LOG(LogROSBridge, Error, TEXT("Socket Error in ROSBridge"));	
			break;
		case ETransportError::ConnectionClosed:
			UE_LOG(LogROSBridge, Error, TEXT("Socket Connection Closed"));	
			break;
		};
	};

	void SetTransportMode(ETransportMode Mode)
	{
		CurrentTransportMode = Mode;
	}

	ETransportMode GetTransportMode() const
	{
		return CurrentTransportMode;
	};
};