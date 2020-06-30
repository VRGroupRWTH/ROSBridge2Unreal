#include "ROSServiceHandle.h"

#include "LogCategory.h"
#include "IRosbridge2Unreal.h"
#include "Services/ROSServiceBase.h"


void UROSServiceHandle::EndPlay(const EEndPlayReason::Type)
{
	Unadvertise(); //Does nothing if not advertised before
}

void UROSServiceHandle::Initialize(FString ServiceName, TSubclassOf<UROSServiceBase> ServiceClass)
{
	IRosbridge2Unreal::Get().EnsureConnectionIsInitialized();
	
	InternalService = IRosbridge2Unreal::Get().GetService(ServiceName, ServiceClass);
}

void UROSServiceHandle::Advertise(TFunction<void (UROSServiceBase*)> Callback)
{
	if(!InternalService){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you Advertise to it."));
		return;
	}
	
	InternalService->Advertise(Callback);
}

void UROSServiceHandle::Advertise()
{
	Advertise([this](UROSServiceBase* Message){OnRequest.Broadcast(Message);});
}

void UROSServiceHandle::Unadvertise()
{
	if(!InternalService) return;
	
	InternalService->Unadvertise();
}


void UROSServiceHandle::Call(const UROSServiceBase* Request, TFunction<void (const UROSServiceBase*)> Callback)
{
	if(!InternalService){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you can Call it."));
		return;
	}
	
	InternalService->CallService(Request, Callback);
}

void UROSServiceHandle::Call(const UROSServiceBase* Request)
{
	if(!RecurrentResponseCallback){
		RegisterResponseCallback([this](const UROSServiceBase* Message){OnResponse.Broadcast(Message);});
	}
	CallRecurrent(Request);
}

void UROSServiceHandle::CallRecurrent(const UROSServiceBase* Request)
{
	if(!InternalService){
		UE_LOG(LogROSBridge, Warning, TEXT("You first have to initialize your ROSServiceHandle before you can Call it."));
		return;
	}
	
	InternalService->CallService(Request, RecurrentResponseCallback);
}

void UROSServiceHandle::RegisterResponseCallback(TFunction<void (const UROSServiceBase*)> Callback)
{
	RecurrentResponseCallback = Callback;
}
