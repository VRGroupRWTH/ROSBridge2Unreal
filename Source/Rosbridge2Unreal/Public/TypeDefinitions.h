#pragma once

#include "CoreMinimal.h"

#include "jsoncons_unreal_wrapper.h"
#include "TypeDefinitions.generated.h"

class UROSMessageBase;
class UROSServiceBase;

using ROSData = jsoncons::ojson;

UENUM()
enum class TransportError : uint8
{
	SocketError,
	ConnectionClosed
};

UENUM()
enum class TransportMode : uint8
{
	JSON UMETA(DisplayName = "JSON Mode"),
	BSON UMETA(DisplayName = "Binary JSON Mode")
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