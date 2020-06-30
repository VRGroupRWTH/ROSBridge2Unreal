#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Services/ROSServiceBase.h"

#include "ROSService.h"

#include "ROSServiceHandle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseSignature, const UROSServiceBase*, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestSignature, UROSServiceBase*, RequestResponse);

UCLASS( ClassGroup=(ROS), meta=(BlueprintSpawnableComponent), Blueprintable )
class ROSBRIDGE2UNREAL_API UROSServiceHandle : public UActorComponent
{
	GENERATED_BODY()

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	UFUNCTION(BlueprintCallable) void Initialize(FString ServiceName, TSubclassOf<UROSServiceBase> ServiceClass);
	UFUNCTION(BlueprintCallable) void Unadvertise();

	void Advertise(TFunction<void (UROSServiceBase*)> Callback);
	void Call(const UROSServiceBase* Request, TFunction<void (const UROSServiceBase*)> Callback);
	void CallRecurrent(const UROSServiceBase* Request);
	void RegisterResponseCallback(TFunction<void (const UROSServiceBase*)> Callback);

private:
	UPROPERTY() UROSService* InternalService;
	TFunction<void(UROSServiceCallMessage& RequestResponse)> InternalRequestCallback = nullptr;
	TFunction<void(const UROSServiceBase*)> RecurrentResponseCallback = nullptr;

	/* 
	 * Blueprint only implementations of C++ functions
	 */
	UPROPERTY(VisibleAnywhere, BlueprintAssignable) FResponseSignature OnResponse;
	UPROPERTY(VisibleAnywhere, BlueprintAssignable) FRequestSignature OnRequest;
	UFUNCTION(BlueprintCallable) void Call(const UROSServiceBase* Request);
	UFUNCTION(BlueprintCallable) void Advertise(); 
};
