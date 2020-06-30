#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"

#include "ROSServiceBase.generated.h"


UCLASS(abstract, hidedropdown, blueprintable, classgroup=ROSServices)
class ROSBRIDGE2UNREAL_API UROSServiceBase : public UObject
{
	GENERATED_BODY()

public:
	/* Message Type */
	virtual FString GetServiceType() PURE_VIRTUAL(UROSServiceBase::GetServiceType, return "";);

	/* Request */
	virtual void RequestToData(ROSData& Message) const PURE_VIRTUAL(UROSServiceBase::RequestToData, return;);
	virtual bool RequestFromData(const ROSData& Message) PURE_VIRTUAL(UROSServiceBase::RequestFromData, return false;);

	/* Response */
	virtual void ResponseToData(ROSData& Message) const PURE_VIRTUAL(UROSServiceBase::ResponseToData, return;);
	virtual bool ResponseFromData(const ROSData& Message) PURE_VIRTUAL(UROSServiceBase::ResponseFromData, return false;);
};

