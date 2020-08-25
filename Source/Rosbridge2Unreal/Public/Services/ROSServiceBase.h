#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"

#include "ROSServiceBase.generated.h"

/**
 * Parent class for all services that are handled by the plugin
 */
UCLASS(abstract, hidedropdown, blueprintable, classgroup=ROSServices)
class ROSBRIDGE2UNREAL_API UROSServiceBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Used as the service type string in the background. Example: "rospy_tutorials/AddTwoInts"
	 * @return The message type as a String
	 */
	virtual FString GetServiceType() PURE_VIRTUAL(UROSServiceBase::GetServiceType, return "";);

	/**
	 * Serialize user request to a message that is sent to ROS Bridge
	 * @param Message - The message to serialize to
	 */
	virtual void RequestToData(ROSData& Message) const PURE_VIRTUAL(UROSServiceBase::RequestToData, return;);
	
	/**
	 * Deserialize a request that comes from the ROS Bridge
	 * @param Message - The message to deserialize
	 * @return Success of deserialization
	 */
	virtual bool RequestFromData(const ROSData& Message) PURE_VIRTUAL(UROSServiceBase::RequestFromData, return false;);

	/**
	 * Serialize the response to a request that is later sent to the ROS Bridge
	 * @param Message - The message to serialize to
	 */
	virtual void ResponseToData(ROSData& Message) const PURE_VIRTUAL(UROSServiceBase::ResponseToData, return;);
	
	/**
	 * Deserialize a service response from the ROS Bridge
	 * @param Message - The message to deserialize
	 * @return Success of deserialization
	 */
	virtual bool ResponseFromData(const ROSData& Message) PURE_VIRTUAL(UROSServiceBase::ResponseFromData, return false;);
};

