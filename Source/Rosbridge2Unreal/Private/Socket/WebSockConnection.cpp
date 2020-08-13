#include "WebSockConnection.h"


#include "HttpModule.h"
#include "LogCategory.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Sockets.h"
#include "jsoncons_unreal_wrapper.h"


bool UWebSockConnection::Initialize(FString IPAddress, int Port, ETransportMode Mode)
{
	Socket = FWebSocketsModule::Get().CreateWebSocket(FString::Printf(TEXT("ws://%s:%d/"), *IPAddress, Port), TEXT("ws"));

	Socket->OnConnectionError().AddLambda([this](const FString & Error) -> void {
	    ReportError(ETransportError::SocketError);
		UE_LOG(LogROSBridge, Error, TEXT("Socket Error Occured: %s"), *Error);
	});
	
	Socket->OnRawMessage().AddUObject(this, &UWebSockConnection::Receive);

	BinaryBuffer.Reserve(65536); //16bit address, used by ROSBridge as a default
	CurrentTransportMode = Mode;

	Socket->Connect();
	
	FPlatformProcess::Sleep( 1);
	
	return true; //Always pretend we are connected. 
}

void UWebSockConnection::Uninitialize()
{
	Socket->Close();
}

bool UWebSockConnection::SendMessage(const uint8_t *Data, const unsigned int Length) const
{
	Socket->Send(Data, Length, CurrentTransportMode == ETransportMode::BSON);
	return true;
}

void UWebSockConnection::Receive(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	if(BinaryBuffer.Num() < Size + BytesRemaining) BinaryBuffer.SetNumUninitialized(Size + BytesRemaining, false);

	FMemory::Memcpy(BinaryBuffer.GetData() + CurrentMessageLength, Data, Size); //Accumulate Message
	CurrentMessageLength += Size;

	if(BytesRemaining > 0) return; //Wait for next call to complete message
	
	ROSData ParsedData;
	if(CurrentTransportMode == ETransportMode::BSON){
		
		try{
			ParsedData = jsoncons::bson::decode_bson<ROSData>(BinaryBuffer.GetData(), BinaryBuffer.GetData() + CurrentMessageLength);
			if (IncomingMessageCallback) IncomingMessageCallback(ParsedData);
		}catch(jsoncons::ser_error e)
		{
			UE_LOG(LogROSBridge, Error, TEXT("Error while parsing BSON message (Ignoring message)"));
		}
		
	} else if(CurrentTransportMode == ETransportMode::JSON) {
		
		try{
			ParsedData = jsoncons::ojson::parse(jsoncons::ojson::string_view_type(reinterpret_cast<char*>(BinaryBuffer.GetData()), CurrentMessageLength));
			if (IncomingMessageCallback) IncomingMessageCallback(ParsedData);
		}catch(jsoncons::ser_error e)
		{
			UE_LOG(LogROSBridge, Error, TEXT("Error while parsing JSON message (Ignoring message): %hs"), e.what());
		}
	}
	
	CurrentMessageLength = 0;	
}
