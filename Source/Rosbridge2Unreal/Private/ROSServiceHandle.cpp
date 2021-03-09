#include "ROSServiceHandle.h"

#include "LogCategory.h"
#include "IRosbridge2Unreal.h"
#include "Services/ROSServiceBase.h"

void UROSServiceHandle::Initialize(FString ServiceName, TSubclassOf<UROSServiceBase> ServiceClass)
{
	if(!ServiceClass)
	{
		UE_LOG(LogROSBridge, Error, TEXT("No class given for initialization of ROSServiceHandle for service %s."), *ServiceName);
		return;
	}
	InternalService = IRosbridge2Unreal::Get().GetService(ServiceName, ServiceClass);
}

void UROSServiceHandle::Advertise(TFunction<void (UROSServiceBase*)> Callback, UROSServiceBase* InReusableRequest)
{
	if(!InternalService){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you Advertise to it."));
		return;
	}
	
	InternalService->Advertise(Callback, InReusableRequest);
}

void UROSServiceHandle::Advertise()
{
	Advertise([this](UROSServiceBase* Message){OnRequest.Broadcast(Message);});
}

void UROSServiceHandle::AdvertiseWithReusableRequest(UROSServiceBase* ReusableRequest)
{
	Advertise([this](UROSServiceBase* Message){OnRequest.Broadcast(Message);},ReusableRequest);
}

void UROSServiceHandle::Unadvertise()
{
	if(!InternalService) return;
	
	InternalService->Unadvertise();
}


void UROSServiceHandle::Call(const UROSServiceBase* Request, TFunction<void (const UROSServiceBase*)> Callback, UROSServiceBase* ReusableResponse)
{
	if(!InternalService){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you can Call it."));
		return;
	}
	
	InternalService->CallService(Request, Callback, ReusableResponse);
}

void UROSServiceHandle::Call(const UROSServiceBase* Request)
{
	if(!RecurrentResponseCallback){
		RegisterResponseCallback([this](const UROSServiceBase* Message){OnResponse.Broadcast(Message);});
	}
	CallRecurrent(Request);
}

void UROSServiceHandle::CallWithReusableResponse(const UROSServiceBase* Request, UROSServiceBase* ReusableResponse)
{
	if(!RecurrentResponseCallback){
		RegisterResponseCallback([this](const UROSServiceBase* Message){OnResponse.Broadcast(Message);});
	}
	CallRecurrent(Request, ReusableResponse);
}

void UROSServiceHandle::CallRecurrent(const UROSServiceBase* Request, UROSServiceBase* ReusableResponse)
{
	if(!InternalService){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you can Call it."));
		return;
	}
	
	InternalService->CallService(Request, RecurrentResponseCallback, ReusableResponse);
}

void UROSServiceHandle::RegisterResponseCallback(TFunction<void (const UROSServiceBase*)> Callback)
{
	RecurrentResponseCallback = Callback;
}
