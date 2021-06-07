#pragma once

#include "CoreMinimal.h"
#include "ROSTopic.h"
#include "Messages/graph_msgs/ROSMsgClock.h"
#include "ROSClockEmitter.generated.h"

/**
 * Emits clock messages into the ros network
 */
UCLASS()
class ROSBRIDGE2UNREAL_API AROSClockEmitter : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AROSClockEmitter();

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "Emit Clock Events from Unreal"))
	bool bEmitClockEvents = false;

	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (DisplayName = "Use Wall Clock Time, else uses the Gameplay Time", EditCondition = "bEmitClockEvents"))
	bool bUseWallClockTime = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Timing Stuff */
	UPROPERTY() UROSTopic* ClockTopic = nullptr;
	UPROPERTY() UROSMsgClock* ClockMessage;
};
