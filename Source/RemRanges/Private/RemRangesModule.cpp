// Copyright RemRemRemRe, All Rights Reserved.

#include "RemRangesModule.h"
#include "Modules/ModuleManager.h"

IRemRangesModule& IRemRangesModule::Get()
{
	return FModuleManager::LoadModuleChecked< IRemRangesModule >( "RemRanges" );
}

bool IRemRangesModule::IsAvailable()
{
	return FModuleManager::Get().IsModuleLoaded( "RemRanges" );
}


class FRemRangesModule final : public IRemRangesModule
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FRemRangesModule, RemRanges)

void FRemRangesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	IRemRangesModule::StartupModule();
}

void FRemRangesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	IRemRangesModule::ShutdownModule();
}
