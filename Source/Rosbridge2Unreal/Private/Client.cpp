// Fill out your copyright notice in the Description page of Project Settings.


#include "Client.h"
#include "ROSMsgString.h"
#include "ROSSrvAddTwoInts.h"

// Sets default values
AClient::AClient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Topic = CreateDefaultSubobject<UROSTopicHandle>("FirstTopic");
	Topic2 = CreateDefaultSubobject<UROSTopicHandle>("SecondTopic");
	Service = CreateDefaultSubobject<UROSServiceHandle>("FirstService");
	Service2 = CreateDefaultSubobject<UROSServiceHandle>("SecondService");
}

// Called when the game starts or when spawned
void AClient::BeginPlay()
{
	Super::BeginPlay();

	Topic->Initialize("/chatter", UROSMsgString::StaticClass());
	Topic2->Initialize("/test_rosbridge2cpp", UROSMsgString::StaticClass());

	Topic->Subscribe(CallbackHelper::Subscribe<UROSMsgString>([](const UROSMsgString* Msg)
	{
		UE_LOG(LogTemp, Log, TEXT("Received: %s"), *Msg->Data);
	}));

	Service->Initialize("/add_two_ints", UROSSrvAddTwoInts::StaticClass());
	Service->RegisterResponseCallback(CallbackHelper::Response<UROSSrvAddTwoInts>([](const UROSSrvAddTwoInts* Rsp)
	{
		UE_LOG(LogTemp, Log, TEXT("Sum of two Ints: %lld"), Rsp->Sum);
	}));

	Service2->Initialize("/add_two_ints_unreal", UROSSrvAddTwoInts::StaticClass());
	Service2->Advertise(CallbackHelper::Advertise<UROSSrvAddTwoInts>([](UROSSrvAddTwoInts* Request)
	{
		Request->Sum = Request->A + Request->B;
	}));
}

// Called every frame
void AClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Topic2->Publish(UROSMsgString::Create("Test" + FString::FromInt(Count)));

	Service->CallRecurrent(UROSSrvAddTwoInts::CreateRequest(Count, 2*Count));
	
	Count++;	
}

