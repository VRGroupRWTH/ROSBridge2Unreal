#pragma once

#include "CoreMinimal.h"

#include "jsoncons_unreal_wrapper.h"
#include "TypeDefinitions.generated.h"

class UROSMessageBase;
class UROSServiceBase;

using ROSData = jsoncons::ojson;

UENUM()
enum class ETransportError : uint8
{
	SocketError,
	ConnectionClosed
};

UENUM()
enum class ETransportMode : uint8
{
	JSON UMETA(DisplayName = "JSON Mode"),
	BSON UMETA(DisplayName = "Binary JSON Mode")
};

UENUM()
enum class EStatusMessageLevel : uint8
{
	Error, //Whenever a user sends a message that is invalid or requests something that does not exist (ie. Sending an incorrect opcode or publishing to a topic that doesn't exist)
	Warning, //error, plus, whenever a user does something that may succeed but the user has still done something incorrectly (ie. Providing a partially-complete published message)
	Info //warning, plus messages indicating success of various operations
};

namespace CallbackHelper
{
	template<class ServiceType>
	static TFunction<void (UROSServiceBase*)> Advertise(TFunction<void(ServiceType*)> Callback)
	{
		return [Callback](UROSServiceBase* IncomingRequest)
		{
			Callback(static_cast<ServiceType*>(IncomingRequest));
		};
	}
	
	template<class ServiceType>
	static TFunction<void (const UROSServiceBase*)> Response(TFunction<void(const ServiceType*)> Callback)
	{
		return [Callback](const UROSServiceBase* IncomingResponse)
		{
			Callback(static_cast<const ServiceType*>(IncomingResponse));
		};
	}

	template<class MessageType>
	static TFunction<void(const UROSMessageBase*)> Subscribe(TFunction<void(const MessageType*)> Callback)
	{
		return [Callback](const UROSMessageBase* IncomingMessage)
		{
			Callback(static_cast<const MessageType*>(IncomingMessage));
		};
	}	
};