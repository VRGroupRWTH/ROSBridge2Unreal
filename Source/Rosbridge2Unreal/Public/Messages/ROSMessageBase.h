#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"

#include "ROSMessageBase.generated.h"


UCLASS(abstract, hidedropdown, blueprintable, classgroup=ROSMessages)
class ROSBRIDGE2UNREAL_API UROSMessageBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void ToData(ROSData& Message) const PURE_VIRTUAL(UROSMessageBase::ToData, return;);
    virtual bool FromData(const ROSData& Message) PURE_VIRTUAL(UROSMessageBase::FromData, return false;);
	virtual FString GetMessageType() PURE_VIRTUAL(UROSMessageBase::GetMessageType, return "";);
};

