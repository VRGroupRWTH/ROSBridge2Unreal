#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"

#include "ROSMessageBase.generated.h"


/**
 * Parent class for all messages that are handled by the plugin
 */
UCLASS(abstract, hidedropdown, blueprintable, classgroup=ROSMessages)
class ROSBRIDGE2UNREAL_API UROSMessageBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Serialize user message to sent to ROS Bridge
	 * @param Message - The message to serialize to
	 */
	virtual void ToData(ROSData& OutMessage) const PURE_VIRTUAL(UROSMessageBase::ToData, return;);

	/**
     * Deserialize message from ROS Bridge
     * @param Message - The message to deserialize
     * @return Success of deserialization
     */
    virtual bool FromData(const ROSData& Message) PURE_VIRTUAL(UROSMessageBase::FromData, return false;);
	
	/**
	 * Used as the message type string in the background. Example: "std_msgs/String"
	 * @return The message type as a String
	 */
	virtual FString GetMessageType() PURE_VIRTUAL(UROSMessageBase::GetMessageType, return "";);
};

