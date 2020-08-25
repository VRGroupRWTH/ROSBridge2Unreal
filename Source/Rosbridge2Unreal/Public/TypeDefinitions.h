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
enum class ESocketMode : uint8
{
	TCP UMETA(DisplayName = "TCP Sockets"),
	WebSocket UMETA(DisplayName = "WebSockets")
};

UENUM()
enum class ETransportMode : uint8
{
	JSON UMETA(DisplayName = "JSON Mode"),
	BSON UMETA(DisplayName = "Binary JSON Mode")
};

namespace CallbackHelper
{
	/**
	 * Convenience function to hide a static cast.
	 * Example usage: `Service->Advertise(CallbackHelper::Advertise<UROSSrvAddTwoInts>([](UROSSrvAddTwoInts* Request){...}));`
	 * @tparam ServiceType - Service class that is responsible for (de-)serialization
	 * @param Callback - The function that processes the request
	 * @return Function that performs the static cast automatically
	 */
	template<class ServiceType>
	static TFunction<void (UROSServiceBase*)> Advertise(TFunction<void(ServiceType*)> Callback)
	{
		return [Callback](UROSServiceBase* IncomingRequest)
		{
			Callback(static_cast<ServiceType*>(IncomingRequest));
		};
	}
	
	/**
	 * Convenience function to hide a static cast.
	 * Example usage: `Service->RegisterResponseCallback(CallbackHelper::Response<UROSSrvAddTwoInts>([](const UROSSrvAddTwoInts* Response){...}));`
	 * @tparam ServiceType - Service class that is responsible for (de-)serialization
	 * @param Callback - The function that processes the response
	 * @return Function that performs the static cast automatically
	 */
	template<class ServiceType>
	static TFunction<void (const UROSServiceBase*)> Response(TFunction<void(const ServiceType*)> Callback)
	{
		return [Callback](const UROSServiceBase* IncomingResponse)
		{
			Callback(static_cast<const ServiceType*>(IncomingResponse));
		};
	}

	/**
	 * Convenience function to hide a static cast.
	 * Example usage: `Topic->Subscribe(CallbackHelper::Subscribe<UROSMsgString>([](const UROSMsgString* Message){...}));`
	 * @tparam MessageType - Message class that is responsible for (de-)serialization
	 * @param Callback - The function that processes the message
	 * @return Function that performs the static cast automatically
	 */
	template<class MessageType>
	static TFunction<void(const UROSMessageBase*)> Subscribe(TFunction<void(const MessageType*)> Callback)
	{
		return [Callback](const UROSMessageBase* IncomingMessage)
		{
			Callback(static_cast<const MessageType*>(IncomingMessage));
		};
	}	
};