#include "ROSClockEmitter.h"

#include "Misc/App.h"
#include "IRosbridge2Unreal.h"
#include "Kismet/GameplayStatics.h"
#include "LogCategory.h"

AROSClockEmitter::AROSClockEmitter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ClockTopic = CreateDefaultSubobject<UROSTopic>("ClockTopic");
}

void AROSClockEmitter::BeginPlay()
{
	Super::BeginPlay();

	/* Check for multiple instances of this class in the level */
	TArray<AActor*> AllEmitters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), AllEmitters);
	if(AllEmitters.Num() > 1 && AllEmitters[0] == this)
	{
	    UE_LOG(LogROSBridge, Warning, TEXT("There are two ClockEmitters present in the Level. This can cause problems later on."));
	}

	/* Setup Topic */
	ClockTopic->Initialize("/clock", UROSMsgClock::StaticClass());
	ClockTopic->Advertise();

	/* Create Reuseable Message */
	ClockMessage = NewObject<UROSMsgClock>(this);
}

void AROSClockEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bEmitClockEvents) return;

	if(bUseWallClockTime)
	{
		const FTimespan WallClockTime = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(0);
		ClockMessage->Seconds = static_cast<int32>(WallClockTime.GetTotalSeconds()); //Implicit floor
		ClockMessage->NanoSeconds = WallClockTime.GetFractionNano();
		ClockTopic->Publish(ClockMessage);
	}
	else
	{
		const float Fraction = DeltaTime - FMath::FloorToInt(DeltaTime);
		ClockMessage->Seconds += FMath::FloorToInt(DeltaTime);
		ClockMessage->NanoSeconds += Fraction * 1000000000ul;
		if(ClockMessage->NanoSeconds > 1000000000ul){
			ClockMessage->Seconds += 1;
			ClockMessage->NanoSeconds -= 1000000000ul;
		}
		ClockTopic->Publish(ClockMessage);
	}
}