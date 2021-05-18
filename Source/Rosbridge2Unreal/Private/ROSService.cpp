#include "ROSService.h"

#include "DataHelpers.h"
#include "IRosbridge2Unreal.h"
#include "Messages/internal/ROSServiceAdvertiseMessage.h"
#include "Messages/internal/ROSServiceUnadvertiseMessage.h"

void UROSService::Initialize(const FString& ServiceName, TSubclassOf<UROSServiceBase> ServiceClass)
{
	if(!ServiceClass)
	{
		UE_LOG(LogROSBridge, Error, TEXT("No class given for initialization of ROSService for service %s."), *ServiceName);
		return;
	}
	
	Super::InitializeConnection();
	StoredServiceName = ServiceName;
	StoredServiceClass = ServiceClass;
}

bool UROSService::Advertise(TFunction<void (UROSServiceBase*)> RequestResponseCallback, UROSServiceBase* InReusableRequest)
{
	if(!bInitialized){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you Advertise to it."));
		return false;
	}
	
	if(bAdvertised) return true; //Already done

	RequestCallback = RequestResponseCallback;
	ReusableRequest = InReusableRequest;

	UROSServiceAdvertiseMessage* ServiceAdvertisement = NewObject<UROSServiceAdvertiseMessage>();
	ServiceAdvertisement->ServiceName = StoredServiceName;
	ServiceAdvertisement->ServiceType = StoredServiceClass.GetDefaultObject()->GetServiceType();
	ServiceAdvertisement->ID = FString::Printf(TEXT("service:%s"), *StoredServiceName);

	bAdvertised = SendMessage(*ServiceAdvertisement);
	return bAdvertised;
}

bool UROSService::Unadvertise()
{
	if(!bAdvertised) return true; //No need to unadvertise
	
	UROSServiceUnadvertiseMessage* ServiceUnadvertisement = NewObject<UROSServiceUnadvertiseMessage>();
	ServiceUnadvertisement->ServiceName = StoredServiceName;
	ServiceUnadvertisement->ID = FString::Printf(TEXT("service:%s"), *StoredServiceName);

	bAdvertised = !SendMessage(*ServiceUnadvertisement);
	return !bAdvertised;
}

bool UROSService::Call(const UROSServiceBase* Request, TFunction<void (const UROSServiceBase*)> Callback, UROSServiceBase* InReusableResponse)
{
	if(!bInitialized){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you can Call it."));
		return false;
	}
	
	if(bAdvertised) return false; //Can't call ourselfes

	UROSServiceCallMessage* ServiceRequest = NewObject<UROSServiceCallMessage>();
	ServiceRequest->ServiceName = StoredServiceName;
	ServiceRequest->ID = FString::Printf(TEXT("call_service:%s:%ld"), *StoredServiceName, GetNextID());
	if(IRosbridge2Unreal::Get().GetSettings()->bFragmentMessages)
	{
		ServiceRequest->FragmentSize = IRosbridge2Unreal::Get().GetSettings()->FragmentSize;
	}
	Request->RequestToData(ServiceRequest->Data);
	
	ResponseCallbacks.Add(ServiceRequest->ID, [this, Callback, InReusableResponse](const UROSServiceResponseMessage& Message)
	{
		UROSServiceBase* ParsedResponse = InReusableResponse; //either reusable or null
		if(!ParsedResponse) ParsedResponse = NewObject<UROSServiceBase>(this, *StoredServiceClass);
		ParsedResponse->ResponseFromData(Message.Data);

		Callback(ParsedResponse);
	});
	
	return SendMessage(*ServiceRequest);
}

void UROSService::IncomingResponse(const UROSServiceResponseMessage& Message)
{
	if(!ResponseCallbacks.Contains(Message.ID)){
		return; //we have not called
	}
	const TFunction<void(const UROSServiceResponseMessage&)> Callback = ResponseCallbacks.FindAndRemoveChecked(Message.ID);
	Callback(Message);
}

void UROSService::IncomingRequest(UROSServiceCallMessage& Message)
{
	UROSServiceResponseMessage* Response = NewObject<UROSServiceResponseMessage>();
	Response->ID = Message.ID;
	Response->ServiceName = Message.ServiceName;
	
	UROSServiceBase* ParsedRequest = ReusableRequest; //either reusable or null
	if(!ParsedRequest) ParsedRequest = NewObject<UROSServiceBase>(this, *StoredServiceClass);
	ParsedRequest->RequestFromData(Message.Data);

	RequestCallback(ParsedRequest);
	ParsedRequest->ResponseToData(Response->Data);
	SendMessage(*Response);
}

FString UROSService::GetServiceName() const
{
	return StoredServiceName;
}

bool UROSService::IsAdvertised() const
{
	return bAdvertised;
}

bool UROSService::HandleMessage(const FString& OPCode, const ROSData& Message)
{
	if(OPCode == "service_response") //Response from service
	{
		UROSServiceResponseMessage* ServiceMessage = NewObject<UROSServiceResponseMessage>();
		if(!ServiceMessage->FromData(Message)) return false;

		if(GetServiceName() == ServiceMessage->ServiceName)
		{
			if(IsAdvertised())
			{
				FString Error;
				DataHelpers::ExtractString(Message, "values", Error);
				UE_LOG(LogROSBridge, Warning, 
					TEXT("We received a response to a known service that we have not called. This can be the result of an error in responding to a request. The response is: %s"),
					*Error
				);
				return false;
			}
			
			IncomingResponse(*ServiceMessage);
			return true;
		}

		UE_LOG(LogROSBridge, Verbose, TEXT("Received response from a service we dont know."));
		return false;
	}

	if(OPCode == "call_service") //call to a service from us
	{
		UROSServiceCallMessage* ServiceMessage = NewObject<UROSServiceCallMessage>();
		if(!ServiceMessage->FromData(Message)) return false;
		
		if(GetServiceName() == ServiceMessage->ServiceName)
		{
			IncomingRequest(*ServiceMessage);
			return true;
		}

		UE_LOG(LogROSBridge, Verbose, TEXT("We were called a service but are not responsible."));
		return false;
	}

	return false;
}

void UROSService::Advertise()
{
	Advertise([this](UROSServiceBase* Message){OnRequest.Broadcast(Message);});
}

void UROSService::AdvertiseWithReusableRequest(UROSServiceBase* InReusableRequest)
{
	Advertise([this](UROSServiceBase* Message){OnRequest.Broadcast(Message);},InReusableRequest);
}

void UROSService::Call(const UROSServiceBase* Request)
{
	if(!RecurrentResponseCallback){
		RegisterResponseCallback([this](const UROSServiceBase* Message){OnResponse.Broadcast(Message);});
	}
	CallRecurrent(Request);
}

void UROSService::CallWithReusableResponse(const UROSServiceBase* Request, UROSServiceBase* InReusableResponse)
{
	if(!RecurrentResponseCallback){
		RegisterResponseCallback([this](const UROSServiceBase* Message){OnResponse.Broadcast(Message);});
	}
	CallRecurrent(Request, InReusableResponse);
}

void UROSService::RegisterResponseCallback(TFunction<void (const UROSServiceBase*)> Callback)
{
	RecurrentResponseCallback = Callback;
}

void UROSService::CallRecurrent(const UROSServiceBase* Request, UROSServiceBase* InReusableResponse)
{
	if(!bInitialized){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you can Call it."));
		return;
	}
	
	Call(Request, RecurrentResponseCallback, InReusableResponse);
}

void UROSService::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	switch(EndPlayReason)
	{
		case EEndPlayReason::LevelTransition:
		case EEndPlayReason::EndPlayInEditor:
		case EEndPlayReason::RemovedFromWorld:
		case EEndPlayReason::Quit:
			Unadvertise();
			UninitializeConnection();
		case EEndPlayReason::Destroyed: break;
	}
}
