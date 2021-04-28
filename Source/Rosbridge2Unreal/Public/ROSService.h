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

	void Initialize(const FString& ServiceName, TSubclassOf<UROSServiceBase> ServiceClass);
	
	bool Advertise(TFunction<void(UROSServiceBase*)> RequestResponseCallback, UROSServiceBase* InReusableRequest = nullptr);

	bool Unadvertise();

	bool CallService(const UROSServiceBase* Request, TFunction<void(const UROSServiceBase*)> Callback, UROSServiceBase* InReusableResponse = nullptr);
	
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

	UPROPERTY() UROSServiceBase* ReusableResponse = nullptr;
	UPROPERTY() UROSServiceBase* ReusableRequest = nullptr;
};