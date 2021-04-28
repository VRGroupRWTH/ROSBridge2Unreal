#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Services/ROSServiceBase.h"

#include "ROSService.h"

#include "ROSServiceHandle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseSignature, const UROSServiceBase*, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestSignature, UROSServiceBase*, RequestResponse);

/**
 * This component can be added to an actor to offer/call a service to the ROS Bridge
 * It serves as a proxy to the internally uses ROSService
 */
UCLASS( ClassGroup=(ROS), meta=(BlueprintSpawnableComponent), Blueprintable )
class ROSBRIDGE2UNREAL_API UROSServiceHandle : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * Initialize handle
	 * @param ServiceName - Service name that is used to advertise/call later. Used as an ID internally.
	 * @param ServiceClass - The service class that is responsible for (de-)serialization of messages
	 */
	UFUNCTION(BlueprintCallable) void Initialize(const FString& ServiceName, TSubclassOf<UROSServiceBase> ServiceClass);
	
	/**
	 * Unadvertise this service from the ROS Bridge (does nothing if not advertised)
	 */
	UFUNCTION(BlueprintCallable) void Unadvertise();

	
	/**
	 * Advertise this service to the ROS Bridge (C++ variant)
	 * @param Callback - Function that should process requests
	 * @param InReusableRequest - Service class instance that is reused (optional)
	 */
	void Advertise(TFunction<void(UROSServiceBase*)> Callback, UROSServiceBase* InReusableRequest = nullptr);
	
	/**
	 * Call the service via the ROS Bridge (C++ variant)
	 * @param Request - Request to send
	 * @param Callback - Function that is called upon response
	 * @param ReusableResponse - Service class instance that is reused (optional)
	 */
	void Call(const UROSServiceBase* Request, TFunction<void (const UROSServiceBase*)> Callback, UROSServiceBase* ReusableResponse = nullptr);
	
	/**
	 * Call the service via the ROSBridge without giving a callback each time (C++ variant)
	 * RegisterResponseCallback should be called to specify the used callback beforehand
	 * @param Request - Request to send
	 * @param ReusableResponse - Service class instance that is reused (optional)
	 */
	void CallRecurrent(const UROSServiceBase* Request, UROSServiceBase* ReusableResponse = nullptr);
	
	/**
	 * Register a callback that is used by the CallRecurrent function (C++ variant)
	 * @param Callback - Function that is called upon response
	 */
	void RegisterResponseCallback(TFunction<void (const UROSServiceBase*)> Callback);

private:
	UPROPERTY() UROSService* InternalService;
	TFunction<void(UROSServiceCallMessage& RequestResponse)> InternalRequestCallback = nullptr;
	TFunction<void(const UROSServiceBase*)> RecurrentResponseCallback = nullptr;

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
	UFUNCTION(BlueprintCallable) void CallWithReusableResponse(const UROSServiceBase* Request, UROSServiceBase* ReusableResponse);
	
	/**
	 * Advertise this service to the ROS Bridge
	 */
	UFUNCTION(BlueprintCallable) void Advertise(); 
	
	/**
	 * Advertise this service to the ROS Bridge and use a reusable service class instance for deserialization
	 * @param ReusableRequest - Service class instance that is reused
	 */
	UFUNCTION(BlueprintCallable) void AdvertiseWithReusableRequest(UROSServiceBase* ReusableRequest); 
};
