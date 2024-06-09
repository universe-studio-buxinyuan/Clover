#pragma once

#include "FCloverPreloadScreen.h"
#include "Misc/App.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(CloverStartupScreen, All, All);

class FCloverStartupScreen final : public IModuleInterface
{
public:
	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;

	bool IsGameModule() const override;

private:
	void OnPreLoadScreenManagerCleanUp();

	TSharedPtr<FCloverPreloadScreen> PreLoadingScreen;
};