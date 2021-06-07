#include "ROSBridgeConnection.h"

#include "Async/Async.h"
#include "DataHelpers.h"
#include "IRosbridge2Unreal.h"
#include "LogCategory.h"
#include "Messages/internal/ROSAuthMessage.h"
#include "jsoncons_unreal_wrapper.h"
#include "Socket/WebSockConnection.h"
#include "Socket/TCPConnection.h"

bool UROSBridgeConnection::InitializeConnection()
{
	const URosbridgeSettings* Settings = IRosbridge2Unreal::Get().GetSettings();
	
	if(Settings->bSimulateConnection)
	{
		bInitialized = true;
		return true; //Don't attempt connection
	}

	if(IRosbridge2Unreal::Get().HasFailedConnection())
	{
		UE_LOG(LogROSBridge, Warning, TEXT("Detected that a failed connection occured earlier. Not attempting reconnection."));
	    return false;
	}

	switch(Settings->SocketMode) {
		case ESocketMode::TCP:
			Connection = NewObject<UTCPConnection>(this);
			break;
		case ESocketMode::WebSocket:
			Connection = NewObject<UWebSockConnection>(this);
			break;
	}

	Connection->RegisterIncomingMessageCallback([this](const ROSData& Message){
		AsyncTask(ENamedThreads::GameThread, [this, Message]() {
			IncomingMessage(Message);
		});
	});
	
	bInitialized = Connection->Initialize(Settings->IP, Settings->Port, Settings->TransportationMode);

	if(!bInitialized)
	{
	    IRosbridge2Unreal::Get().ReportFailedConnection();
	}

	if(Settings->bShouldAuthenticate && bInitialized)
	{
		UROSAuthMessage* AuthMsg = NewObject<UROSAuthMessage>(this);
		AuthMsg->Client = FString::Printf(TEXT("UnrealClient:%s"),FApp::GetProjectName());
		AuthMsg->Destination = Settings->IP;
		AuthMsg->Secret = Settings->Secret;
		SendMessage(*AuthMsg);
	}
	
	return bInitialized;
}

void UROSBridgeConnection::UninitializeConnection()
{
	bInitialized = false;
	
	if(Connection)
	{
		Connection->Uninitialize();
	}
}

long UROSBridgeConnection::GetNextID()
{
	return ++IDCounter;
}

bool UROSBridgeConnection::SendMessage(const UROSBridgeMessage& Message) const
{
	if(IRosbridge2Unreal::Get().GetSettings()->bSimulateConnection) return true;
	if(!bInitialized) return false;
	
	ROSData Data;
	Message.ToData(Data);
	if(Connection->SendMessage(Data))
	{
		UE_LOG(LogROSBridge, VeryVerbose, TEXT("Sent ROSBridge message: %s"), *DataHelpers::Internal::DataToString(Data));
		return true;
	}
	else
	{
		UE_LOG(LogROSBridge, Warning, TEXT("Error sending ROSBridge message: %s"), *DataHelpers::Internal::DataToString(Data));
	}
	
	return false;
}

void UROSBridgeConnection::IncomingMessage(const ROSData& Message)
{
	if(!bInitialized) return; //Ignore message that came after closing
	
	FString OPCode;

	if(!DataHelpers::Extract<FString>(Message, "op", OPCode)){
		UE_LOG(LogROSBridge, Warning, TEXT("Received message without op code."));	
		return;
	}

	if(OPCode == "fragment") //fragmented message
	{
		FString Id, Data;
		int64 Num, Total, NumCharacters = 0;
		DataHelpers::Extract<FString>(Message, "id", Id);
		DataHelpers::Extract<FString>(Message, "data", Data);
		DataHelpers::Extract<int64>(Message, "num", Num);
		DataHelpers::Extract<int64>(Message, "total", Total);
		TArray<FString>& Fragments = MessageFragments.FindOrAdd(Id);
		Fragments.SetNum(Total);
		Fragments[Num] = Data;
		for(FString& Fragment : Fragments)
		{
			NumCharacters += Fragment.Len();
			if(Fragment.IsEmpty()) return; //At least one is missing
		}

		/* Message complete */
		FString CompleteMessage = "";
		CompleteMessage.Reserve(NumCharacters);
		for(const FString& Fragment : Fragments)
		{
			CompleteMessage += Fragment;
		}
		
		ROSData ParsedData;
		try{
			ParsedData = jsoncons::ojson::parse(jsoncons::ojson::string_view_type(TCHAR_TO_UTF8(*CompleteMessage), NumCharacters));
			MessageFragments.Remove(Id); //Parsed completely
			this->IncomingMessage(ParsedData);
		}catch(jsoncons::ser_error e)
		{
			UE_LOG(LogROSBridge, Error, TEXT("Error while parsing JSON message (Ignoring message): %hs"), e.what());
		}
		return;
	}
	
	/* Leave actual handling to subclasses */
	if(HandleMessage(OPCode, Message)){ 
		return;
	}
	
	UE_LOG(LogROSBridge, Warning, TEXT("Received message with OP-Code '%s', which is not supported (yet)"), *OPCode);
}

/*
 * Message Queueing: TODO
 * Implemented, but neither used nor tested 
 */

void UROSBridgeConnection::QueueMessage(UROSBridgeMessage* Message)
{
	if(!bInitialized) return; //Ignore message that came after closing
	
	MutexMessageQueue.Lock();
	
	QueuedMessages.Push(Message);
	
	MutexMessageQueue.Unlock();
}

uint32 UROSBridgeConnection::Run()
{
	while(KeepSenderThreadRunning)
	{
		if(QueuedMessages.Num() <= 0){
			FPlatformProcess::Sleep(0.01); //Not poll to fast
			continue;
		}
		
		MutexMessageQueue.Lock();
		UROSBridgeMessage* Message = QueuedMessages.Pop(false);
		MutexMessageQueue.Unlock();
		
		SendMessage(*Message);
	}

	return EXIT_SUCCESS;
}