#include "ROSClockEmitter.h"


#include "Misc/App.h"
#include "Async/Async.h"
#include "Messages/internal/ROSBridgeMessage.h"
#include "DataHelpers.h"
#include "IRosbridge2Unreal.h"
#include "LogCategory.h"
#include "Messages/internal/ROSAuthMessage.h"
#include "Socket/WebSockConnection.h"
#include "jsoncons_unreal_wrapper.h"

void UROSClockEmitter::TickEvent(float DeltaTime)
{	
	if(!ClockTopic)
	{
		ClockTopic = NewObject<UROSTopic>(this);
		ClockTopic->Initialize("/clock", UROSMsgClock::StaticClass());
		ClockTopic->Advertise();
	}

	if(!ClockMessage){
		ClockMessage = NewObject<UROSMsgClock>(this);
	}

	if(!bSetUpdateIntervalSettings)
	{
		FApp::SetFixedDeltaTime(IRosbridge2Unreal::Get().GetSettings()->FixedUpdateInterval);
		FApp::SetUseFixedTimeStep(IRosbridge2Unreal::Get().GetSettings()->bUseFixedUpdateInterval);
		bSetUpdateIntervalSettings = true;
	}
	
	if(IRosbridge2Unreal::Get().GetSettings()->bUseWallClockTime)
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

bool UROSClockEmitter::HandleMessage(const FString& OPCode, const ROSData& Message)
{
	UE_LOG(LogROSBridge, Warning, TEXT("Clock Emitter received a message. This should not happen."));
	return false;
}
