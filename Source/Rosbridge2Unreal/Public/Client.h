#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSTopic.h"
#include "ROSService.h"

#include "Client.generated.h"


UCLASS()
class AClient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY() UROSTopic* ReceiveTopic;
	UPROPERTY() UROSTopic* SendTopic;
	UPROPERTY() UROSService* ExternalService;
	UPROPERTY() UROSService* OfferedService;
	uint64 Count = 0;
};
