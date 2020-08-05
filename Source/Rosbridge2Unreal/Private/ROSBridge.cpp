#include "ROSBridge.h"


#include "Misc/App.h"
#include "Async/Async.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "DataHelpers.h"
#include "LogCategory.h"
#include "ROSAuthMessage.h"

DEFINE_LOG_CATEGORY(LogROSBridge);

bool UROSBridge::Initialize()
{
	Settings = GetDefault<URosbridgeSettings>();
	bSettingsRead = true;
	
	if(Settings->bSimulateConnection) return true; //Don't attempt connection
	
	Connection = NewObject<UTCPConnection>(this);
	Connection->RegisterIncomingMessageCallback([this](const ROSData& Message){
		AsyncTask(ENamedThreads::GameThread, [this, Message]() {
			IncomingMessage(Message);
		});
	});
	
	const bool ConnectionInitialized = Connection->Initialize(TCHAR_TO_UTF8(*Settings->IP), Settings->Port, Settings->TransportationMode);
	
	SenderThread = FRunnableThread::Create(this, TEXT("ROSBridgeSenderThread"), 0, TPri_Normal);

	bInitialized = true;

	if(Settings->bShouldAuthenticate)
	{
		UROSAuthMessage* AuthMsg = NewObject<UROSAuthMessage>(this);
		AuthMsg->Client = Connection->GetOwnIPAddress();
		AuthMsg->Destination = Settings->IP;
		AuthMsg->Secret = Settings->Secret;
		SendMessage(*AuthMsg);
	}
	
	return ConnectionInitialized;
}

void UROSBridge::Uninitialize()
{
	bInitialized = false;
	
	//Unsubscribe/Unadvertise from everything
	
	for(UROSTopic*& Topic : Topics)
	{
		Topic->ForceUnsubscribeInternal();
	}
	Topics.Empty();

	for(UROSService*& Service : Services)
	{
		Service->Unadvertise();
	}
	Services.Empty();
	
	KeepSenderThreadRunning = false;
	if (SenderThread) {
		SenderThread->WaitForCompletion();
	}
	if(Connection)
	{
		Connection->Uninitialize();
	}
}

bool UROSBridge::SendMessage(const FString& Data) const
{
	if(bSettingsRead && Settings->bSimulateConnection) return true;
	if(!bInitialized) return true; /* return true, since this is no error */
	return Connection->SendMessage(TCHAR_TO_UTF8(*Data));
}

long UROSBridge::GetNextID()
{
	return ++IDCounter;
}

bool UROSBridge::IsBSONMode() const
{
	return Connection->GetTransportMode() == TransportMode::BSON;
}

bool UROSBridge::SendMessage(UROSBridgeMessage& Message) const
{
	if(bSettingsRead && Settings->bSimulateConnection) return true;
	if(!bInitialized) return true; /* return true, since this is no error */
	
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

UROSTopic* UROSBridge::GetTopic(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass)
{
	UROSTopic** FoundTopic = Topics.FindByPredicate([TopicName](const UROSTopic* Topic)
	{
		return Topic->GetTopicName() == TopicName;
	});

	if(FoundTopic) return *FoundTopic;

	UROSTopic* NewTopic = NewObject<UROSTopic>(this);
	NewTopic->Initialize(TopicName, MessageClass);

	Topics.Add(NewTopic);
	return NewTopic;
}

UROSService* UROSBridge::GetService(FString ServiceName, TSubclassOf<UROSServiceBase> ServiceClass)
{
	UROSService** FoundService = Services.FindByPredicate([ServiceName](const UROSService* Service)
	{
		return Service->GetServiceName() == ServiceName;
	});

	if(FoundService) return *FoundService;

	UROSService* NewService = NewObject<UROSService>(this);
	NewService->Initialize(ServiceName, ServiceClass);

	Services.Add(NewService);
	return NewService;
}

void UROSBridge::TickEvent(float DeltaTime)
{
	if(!bSettingsRead || !Settings->bEmitClockEvents || !bInitialized) return; //Nothing to do here
	
	if(!ClockTopic)
	{
		ClockTopic = GetTopic("/clock", UROSMsgClock::StaticClass());
		ClockTopic->Advertise();
	}

	if(!ClockMessage){
		ClockMessage = NewObject<UROSMsgClock>(this);
	}

	if(!bSetUpdateIntervalSettings)
	{
		FApp::SetFixedDeltaTime(Settings->FixedUpdateInterval);
		FApp::SetUseFixedTimeStep(Settings->bUseFixedUpdateInterval);
		bSetUpdateIntervalSettings = true;
	}
	
	if(Settings->bUseWallClockTime)
	{
		const FTimespan WallClockTime = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(0);
		ClockMessage->Seconds = static_cast<int32>(WallClockTime.GetTotalSeconds()); //Implicit floor
		ClockMessage->NanoSeconds = WallClockTime.GetFractionNano();
		ClockTopic->Publish(ClockMessage);
	}
	else
	{
		const float Fraction = DeltaTime - FMath::FloorToInt(DeltaTime);
		ClockMessage->Seconds += FMath::FloorToInt(DeltaTime);
		ClockMessage->NanoSeconds += Fraction * 1000000000ul;
		if(ClockMessage->NanoSeconds > 1000000000ul){
			ClockMessage->Seconds += 1;
			ClockMessage->NanoSeconds -= 1000000000ul;
		}
		ClockTopic->Publish(ClockMessage);
	}
}

void UROSBridge::QueueMessage(UROSBridgeMessage* Message)
{
	if(!bInitialized) return; //Ignore message that came after closing
	
	MutexMessageQueue.Lock();
	
	QueuedMessages.Push(Message);
	
	MutexMessageQueue.Unlock();
}

uint32 UROSBridge::Run()
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

void UROSBridge::IncomingMessage(const ROSData& Message)
{
	if(!bInitialized) return; //Ignore message that came after closing
	
	FString OPCode;

	if(!DataHelpers::ExtractString(Message, "op", OPCode)){
		UE_LOG(LogROSBridge, Warning, TEXT("Received message without op code."));	
		return;
	}

	if(OPCode == "publish") //Message for Topic
	{
		UROSTopicPublishMessage* TopicMessage = NewObject<UROSTopicPublishMessage>();
		if(!TopicMessage->FromData(Message)) return;
		
		for(UROSTopic* Topic : Topics)
		{
			if(Topic->GetTopicName() == TopicMessage->TopicName)
			{
				Topic->IncomingMessage(*TopicMessage);
				return;
			}
		}

		UE_LOG(LogROSBridge, Verbose, TEXT("Received message for a topic we dont know."));
		return;
	}

	if(OPCode == "service_response") //Response from service
	{
		UROSServiceResponseMessage* ServiceMessage = NewObject<UROSServiceResponseMessage>();
		if(!ServiceMessage->FromData(Message)) return;

		for(UROSService* Service : Services)
		{
			if(Service->GetServiceName() == ServiceMessage->ServiceName)
			{
				if(Service->IsAdvertised())
				{
					FString Error;
					DataHelpers::ExtractString(Message, "values", Error);
					UE_LOG(LogROSBridge, Warning, 
						TEXT("We received a response to a known service that we have not called. This can be the result of an error in responding to a request. The response is: %s"),
						*Error
					);
					return;
				}
				
				Service->IncomingResponse(*ServiceMessage);
				return;
			}
		}

		UE_LOG(LogROSBridge, Verbose, TEXT("Received response from a service we dont know."));
		return;
	}

	if(OPCode == "call_service") //call to a service from us
	{
		UROSServiceCallMessage* ServiceMessage = NewObject<UROSServiceCallMessage>();
		if(!ServiceMessage->FromData(Message)) return;
		
		for(UROSService* Service : Services)
		{
			if(Service->GetServiceName() == ServiceMessage->ServiceName)
			{
				Service->IncomingRequest(*ServiceMessage);
				return;
			}
		}
		return;
	}

	UE_LOG(LogROSBridge, Warning, TEXT("Received message with OP-Code '%s', which is not supported (yet)"), *OPCode);
}
