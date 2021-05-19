#pragma once

#include "Modules/ModuleManager.h"
#include "RosbridgeSettings.h"

class ROSBRIDGE2UNREAL_API IRosbridge2Unreal : public IModuleInterface
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
	 * Get Settings for the ROSBridge
	 * @return immutable URosbridgeSettings object
	 */
	static const URosbridgeSettings* GetSettings()
	{
		static const URosbridgeSettings* Settings = GetDefault<URosbridgeSettings>();
		return Settings;
	};
};
