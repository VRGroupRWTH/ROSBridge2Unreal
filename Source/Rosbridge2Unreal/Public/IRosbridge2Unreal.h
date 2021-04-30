#pragma once

#include "Modules/ModuleManager.h"
#include "ROSBridge.h"
#include "Messages/ROSMessageBase.h"
#include "ROSTopic.h"

class IRosbridge2Unreal : public IModuleInterface
{
public:
	/**
	* Singleton-like access to this module's interface.
	* @return Returns singleton instance, loading the module on demand if needed.
	*/
	static inline IRosbridge2Unreal& Get()
	{
		return FModuleManager::LoadModuleChecked<IRosbridge2Unreal>("Rosbridge2Unreal");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	* @return True if the module is loaded and ready to use.
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("Rosbridge2Unreal");
	}

	/**
	 * Retrieve the Singleton instance. Does not require to be on the GameThread, but breaks if not loaded.
	 * @return Returns singleton instance
	 */
	static inline IRosbridge2Unreal& GetUnchecked()
	{
		IRosbridge2Unreal* Interface = FModuleManager::GetModulePtr<IRosbridge2Unreal>("Rosbridge2Unreal");
		check(Interface);
		return *Interface;
	}

	/**
	 * Reads settings from project settings and initializes the connection
	 * @return Connection successful
	 */
	virtual bool InitializeConnection() = 0;

	/**
	 * Checks if we are connected.
	 * @return Connection successful
	 */
	virtual bool IsConnected() = 0;

	/**
	 * @return Check if connection is in BSON mode (True if BSON is used, false if JSON)
	 */
	virtual bool IsBSONMode() = 0;

	/**
	 * @return Get handle to the ROSBridge
	 */
	virtual UROSBridge* GetBridge() = 0;

	/**
	 * Retrieves a topic instance from the already registered ones. Creates a new one of not present so far.
	 * @param TopicName - The topic name that is used for advertise/subscribe etc.
	 * @param MessageClass - The message class that is used for (de-)serialization of messages
	 * @return The topic instance
	 */
	virtual UROSTopic* GetTopic(const FString& TopicName, TSubclassOf<UROSMessageBase> MessageClass) = 0;

	/**
	 * Retrieves a service instance from the already registered ones. Creates a new one of not present so far.
	 * @param ServiceName - The service name that is used for advertise/call etc.
	 * @param ServiceClass - The service class that is used for (de-)serialization of requests/responses
	 * @return The service instance
	 */
	virtual UROSService* GetService(const FString& ServiceName,  TSubclassOf<UROSServiceBase> ServiceClass) = 0;

	/**
	 * Serialize and send message to the ROS Bridge
	 * @param Message - The message to send
	 * @return Success of sending operation
	 */
	virtual bool SendMessage(const UROSBridgeMessage& Message) = 0;

	/**
	 * Generated one time IDs that can be used in successive messages
	 * @return Successive one time IDs
	 */
	virtual long GetNextID() = 0;

	/**
	 * Get Settings for the ROSBridge
	 * @return immutable URosbridgeSettings object
	 */
	virtual const URosbridgeSettings* GetSettings() = 0;
};
