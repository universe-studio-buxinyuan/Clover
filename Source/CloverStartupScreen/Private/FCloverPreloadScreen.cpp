#include "FCloverPreloadScreen.h"
#include "Misc/App.h"
#include "SPreloadingScreenWidget.h"

#define LOCTEXT_NAMESPACE "FCloverPreloadScreen"

void FCloverPreloadScreen::Init()
{
	if (!GIsEditor && FApp::CanEverRender())
	{
		EngineLoadingWidget = SNew(SPreloadingScreenWidget);
	}
}

#undef LOCTEXT_NAMESPACE