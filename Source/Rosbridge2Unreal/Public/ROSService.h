#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"

#include "Services/ROSServiceBase.h"
#include "Messages/internal/ROSServiceCallMessage.h"
#include "Messages/internal/ROSServiceResponseMessage.h"
#include "ROSBridgeConnection.h"
#include "ROSService.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseSignature, const UROSServiceBase*, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestSignature, UROSServiceBase*, RequestResponse);

/*
 *	This component can be added to an actor to offer/call a service to the ROS Bridge
 */
UCLASS( ClassGroup=(ROS), meta=(BlueprintSpawnableComponent), Blueprintable )
class ROSBRIDGE2UNREAL_API UROSService : public UROSBridgeConnection
{
	GENERATED_BODY()
	
public:	
	UROSService(){};

	/**
	 * Initialize handle
	 * @param ServiceName - Service name that is used to advertise/call later. Used as an ID internally.
	 * @param ServiceClass - The service class that is responsible for (de-)serialization of messages
	 */
	UFUNCTION(BlueprintCallable) void Initialize(const FString& ServiceName, TSubclassOf<UROSServiceBase> ServiceClass);

	/**
	 * Advertise this service to the ROS Bridge (C++ variant)
	 * @param Callback - Function that should process requests
	 * @param InReusableRequest - Service class instance that is reused (optional)
	 */
	bool Advertise(TFunction<void(UROSServiceBase*)> RequestResponseCallback, UROSServiceBase* InReusableRequest = nullptr);

	/**
	 * Unadvertise this service from the ROS Bridge (does nothing if not advertised)
	 */
	UFUNCTION(BlueprintCallable) bool Unadvertise();

	/**
	 * Call the service via the ROS Bridge (C++ variant)
	 * @param Request - Request to send
	 * @param Callback - Function that is called upon response
	 * @param ReusableResponse - Service class instance that is reused (optional)
	 */
	bool Call(const UROSServiceBase* Request, TFunction<void(const UROSServiceBase*)> Callback, UROSServiceBase* InReusableResponse = nullptr);
	

	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetServiceName() const;
	UFUNCTION(BlueprintCallable, BlueprintPure) bool IsAdvertised() const;

	/**
	 * Register a callback that is used by the CallRecurrent function (C++ variant)
	 * @param Callback - Function that is called upon response
	 */
	void RegisterResponseCallback(TFunction<void (const UROSServiceBase*)> Callback);

	/**
	 * Call the service via the ROSBridge without giving a callback each time (C++ variant)
	 * RegisterResponseCallback should be called to specify the used callback beforehand
	 * @param Request - Request to send
	 * @param ReusableResponse - Service class instance that is reused (optional)
	 */
	void CallRecurrent(const UROSServiceBase* Request, UROSServiceBase* ReusableResponse = nullptr);

	/*
	 * Undavertises and kills connection on Endplay
	 */
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	
private:
	void IncomingResponse(const UROSServiceResponseMessage& Message);
	void IncomingRequest(UROSServiceCallMessage& Message);
	bool HandleMessage(const FString& OPCode, const ROSData& Message) override;
	
	TMap<FString, TFunction<void(const UROSServiceResponseMessage& Message)>> ResponseCallbacks;
	TFunction<void (UROSServiceBase*)> RequestCallback;
	
	FString StoredServiceName = "";
	TSubclassOf<UROSServiceBase> StoredServiceClass;
	bool bAdvertised = false;

	UPROPERTY() UROSServiceBase* ReusableResponse = nullptr;
	UPROPERTY() UROSServiceBase* ReusableRequest = nullptr;

	/**
	 * Advertise this service to the ROS Bridge
	 */
	UFUNCTION(BlueprintCallable) void Advertise(); 
	
	/**
	 * Advertise this service to the ROS Bridge and use a reusable service class instance for deserialization
	 * @param InReusableRequest - Service class instance that is reused
	 */
	UFUNCTION(BlueprintCallable) void AdvertiseWithReusableRequest(UROSServiceBase* InReusableRequest); 
	
	/**
	 * Event that is called upon a response is received
	 */
	UPROPERTY(VisibleAnywhere, BlueprintAssignable) FResponseSignature OnResponse;
	
	/**
	 * Event that is called upon a request is received
	 */
	UPROPERTY(VisibleAnywhere, BlueprintAssignable) FRequestSignature OnRequest;

	/**
	 * Call the service via the ROS Bridge (Blueprint variant)
	 * @param Request - Request to send
	 */
	UFUNCTION(BlueprintCallable) void Call(const UROSServiceBase* Request);
	
	/**
	 * Call the service via the ROS Bridge, but use a reuseable service class instance for deserialization (Blueprint variant)
	 * @param Request - Request to send
	 * @param ReusableResponse - Service class instance that is reused
	 */
	UFUNCTION(BlueprintCallable) void CallWithReusableResponse(const UROSServiceBase* Request, UROSServiceBase* InReusableResponse);

	TFunction<void(const UROSServiceBase*)> RecurrentResponseCallback = nullptr;
};