#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ROSTopic.h"
#include "ROSService.h"
#include "Messages/graph_msgs/ROSMsgClock.h"
#include "ROSClockEmitter.generated.h"

/**
 * Emits clock messages into the ros network
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSClockEmitter : public UROSBridgeConnection
{
	GENERATED_BODY()
public:
	/**
	 * Used internally to send clock events
	 */
	void TickEvent(float DeltaTime);
	
private:

	/* Timing Stuff */
	UPROPERTY() UROSTopic* ClockTopic = nullptr;
	UPROPERTY() UROSMsgClock* ClockMessage;
	bool bSetUpdateIntervalSettings = false;

	bool HandleMessage(const FString& OPCode, const ROSData& Message) override;
};
