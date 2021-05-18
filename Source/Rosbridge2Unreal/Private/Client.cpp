#include "Client.h"

#include "Engine/Engine.h"
#include "Messages/std_msgs/ROSMsgString.h"
#include "Services/rospy_tutorials/ROSSrvAddTwoInts.h"

// Sets default values
AClient::AClient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ReceiveTopic = CreateDefaultSubobject<UROSTopic>("ReceiveTopic");
	SendTopic = CreateDefaultSubobject<UROSTopic>("SendTopic");
	ExternalService = CreateDefaultSubobject<UROSService>("ExternalService");
	OfferedService = CreateDefaultSubobject<UROSService>("OfferedService");
}

// Called when the game starts or when spawned
void AClient::BeginPlay()
{
	Super::BeginPlay();

	ReceiveTopic->Initialize("/chatter", UROSMsgString::StaticClass());
	SendTopic->Initialize("/chatter_unreal", UROSMsgString::StaticClass());
	
	ReceiveTopic->Subscribe(CallbackHelper::Subscribe<UROSMsgString>([](const UROSMsgString* Msg)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Cyan, "Received: " + Msg->Data);
	}));

	ExternalService->Initialize("/add_two_ints", UROSSrvAddTwoInts::StaticClass());
	ExternalService->RegisterResponseCallback(CallbackHelper::Response<UROSSrvAddTwoInts>([](const UROSSrvAddTwoInts* Rsp)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Cyan, "Sum of two Ints: " + FString::FromInt(Rsp->Sum));
	}));

	OfferedService->Initialize("/add_two_ints_unreal", UROSSrvAddTwoInts::StaticClass());
	OfferedService->Advertise(CallbackHelper::Advertise<UROSSrvAddTwoInts>([](UROSSrvAddTwoInts* Request)
	{
		Request->Sum = Request->A + Request->B;
	}));
}

// Called every frame
void AClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SendTopic->Publish(UROSMsgString::Create("Test" + FString::FromInt(Count)));

	ExternalService->CallRecurrent(UROSSrvAddTwoInts::CreateRequest(Count, 2*Count));
	
	Count++;	
}

