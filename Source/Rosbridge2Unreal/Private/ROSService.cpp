#include "ROSService.h"
#include "IRosbridge2Unreal.h"
#include "Messages/internal/ROSServiceAdvertiseMessage.h"
#include "Messages/internal/ROSServiceUnadvertiseMessage.h"

void UROSService::Initialize(const FString& ServiceName, TSubclassOf<UROSServiceBase> ServiceClass)
{
	StoredServiceName = ServiceName;
	StoredServiceClass = ServiceClass;
}

bool UROSService::Advertise(TFunction<void (UROSServiceBase*)> RequestResponseCallback, UROSServiceBase* InReusableRequest)
{
	if (bAdvertised) return true; //Already done

	RequestCallback = RequestResponseCallback;
	ReusableRequest = InReusableRequest;

	UROSServiceAdvertiseMessage* ServiceAdvertisement = NewObject<UROSServiceAdvertiseMessage>();
	ServiceAdvertisement->ServiceName = StoredServiceName;
	ServiceAdvertisement->ServiceType = StoredServiceClass.GetDefaultObject()->GetServiceType();
	ServiceAdvertisement->ID = FString::Printf(TEXT("service:%s"), *StoredServiceName);

	bAdvertised = IRosbridge2Unreal::Get().SendMessage(*ServiceAdvertisement);
	return bAdvertised;
}

bool UROSService::Unadvertise()
{
	if (!bAdvertised) return true; //No need to unadvertise
	
	UROSServiceUnadvertiseMessage* ServiceUnadvertisement = NewObject<UROSServiceUnadvertiseMessage>();
	ServiceUnadvertisement->ServiceName = StoredServiceName;
	ServiceUnadvertisement->ID = FString::Printf(TEXT("service:%s"), *StoredServiceName);

	bAdvertised = !IRosbridge2Unreal::Get().SendMessage(*ServiceUnadvertisement);
	return !bAdvertised;
}

bool UROSService::CallService(const UROSServiceBase* Request, TFunction<void (const UROSServiceBase*)> Callback, UROSServiceBase* InReusableResponse)
{
	if (bAdvertised) return false; //Can't call ourselfes

	UROSServiceCallMessage* ServiceRequest = NewObject<UROSServiceCallMessage>();
	ServiceRequest->ServiceName = StoredServiceName;
	ServiceRequest->ID = FString::Printf(TEXT("call_service:%s:%ld"), *StoredServiceName, IRosbridge2Unreal::Get().GetNextID());
	Request->RequestToData(ServiceRequest->Data);
	
	ResponseCallbacks.Add(ServiceRequest->ID, [this, Callback, InReusableResponse](const UROSServiceResponseMessage& Message)
	{
		UROSServiceBase* ParsedResponse = InReusableResponse; //either reusable or null
		if (!ParsedResponse) ParsedResponse = NewObject<UROSServiceBase>(this, *StoredServiceClass);
		ParsedResponse->ResponseFromData(Message.Data);

		Callback(ParsedResponse);
	});
	
	return IRosbridge2Unreal::Get().SendMessage(*ServiceRequest);
}

void UROSService::IncomingResponse(const UROSServiceResponseMessage& Message)
{
	if (!ResponseCallbacks.Contains(Message.ID))
	{
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
	if (!ParsedRequest) ParsedRequest = NewObject<UROSServiceBase>(this, *StoredServiceClass);
	ParsedRequest->RequestFromData(Message.Data);

	RequestCallback(ParsedRequest);
	ParsedRequest->ResponseToData(Response->Data);
	IRosbridge2Unreal::Get().SendMessage(*Response);
}

FString UROSService::GetServiceName() const
{
	return StoredServiceName;
}

bool UROSService::IsAdvertised() const
{
	return bAdvertised;
}
