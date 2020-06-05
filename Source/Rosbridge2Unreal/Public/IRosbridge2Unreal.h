#pragma once

#include "ModuleManager.h"
#include "ROSBridge.h"
#include "ROSMessageBase.h"
#include "ROSTopic.h"

class IRosbridge2Unreal : public IModuleInterface
{
public:
	/**
	* Singleton-like access to this module's interface.
	*
	* @return Returns singleton instance, loading the module on demand if needed.
	*/
	static inline IRosbridge2Unreal& Get()
	{
		return FModuleManager::LoadModuleChecked< IRosbridge2Unreal >("Rosbridge2Unreal");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use.
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("Rosbridge2Unreal");
	}

	static inline IRosbridge2Unreal& GetUnchecked()
	{
		IRosbridge2Unreal* Interface = FModuleManager::GetModulePtr<IRosbridge2Unreal>("Rosbridge2Unreal");
		check(Interface);
		return *Interface;
	}

	/* Initializes with Settings in Project */
	virtual void InitializeConnection() = 0;

	/* Initializes with IP:Port */
	virtual void InitializeConnection(FString IPAddress, int Port, bool bBSONMode = false, bool SimulateConnection = false) = 0;

	/* Ensure connection is made */
	virtual void EnsureConnection() = 0;

	/* True if BSON is used, false if JSON */
	virtual bool IsBSONMode() = 0;

	/* Return a handle to the ROSBridge */
	virtual UROSBridge* GetBridge() = 0;

	/* Get or create an internal TopicHandle and return it */
	virtual UROSTopic* GetTopic(FString TopicName, TSubclassOf<UROSMessageBase> MessageClass) = 0;

	/* Get or create an internal ServiceHandle and return it */
	virtual UROSService* GetService(FString ServiceName,  TSubclassOf<UROSServiceBase> ServiceClass) = 0;

	/* Send an internal Message to the other side */
	virtual bool SendMessage(UROSBridgeMessage& Message) = 0;

	/* IDs used for internal sending */
	virtual long GetNextID() = 0;
};
