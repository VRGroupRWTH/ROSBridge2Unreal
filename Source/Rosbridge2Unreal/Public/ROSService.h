// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"

#include "Services/ROSServiceBase.h"
#include "Messages/internal/ROSServiceCallMessage.h"
#include "Messages/internal/ROSServiceResponseMessage.h"
#include "ROSService.generated.h"

UCLASS()
class UROSService : public UObject
{
	GENERATED_BODY()
	
public:	
	UROSService(){};

	void Initialize(FString ServiceName, TSubclassOf<UROSServiceBase> ServiceClass);
	
	bool Advertise(TFunction<void (UROSServiceBase*)> RequestResponseCallback);

	bool Unadvertise();

	bool CallService(const UROSServiceBase* Request, TFunction<void (const UROSServiceBase*)> Callback);
	
	void IncomingResponse(const UROSServiceResponseMessage& Message);
	void IncomingRequest(UROSServiceCallMessage& Message);

	FString GetServiceName() const;
	bool IsAdvertised() const;
	
private:
	TMap<FString, TFunction<void(const UROSServiceResponseMessage& Message)>> ResponseCallbacks;
	TFunction<void (UROSServiceBase*)> RequestCallback;
	
	FString StoredServiceName = "";
	TSubclassOf<UROSServiceBase> StoredServiceClass;
	bool bAdvertised = false;
};