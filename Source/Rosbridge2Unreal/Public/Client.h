// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSTopicHandle.h"
#include "ROSServiceHandle.h"

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

	UROSTopicHandle* Topic;
	UROSTopicHandle* Topic2;
	UROSServiceHandle* Service;
	UROSServiceHandle* Service2;
	uint64 Count = 0;
};
