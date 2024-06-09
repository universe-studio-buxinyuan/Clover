// Copyright Epic Games, Inc. All Rights Reserved.

#include "CloverStartupScreen.h"
#include "PreLoadScreenManager.h"

DEFINE_LOG_CATEGORY(CloverStartupScreen);

#define LOCTEXT_NAMESPACE "FCloverStartupScreen"

void FCloverStartupScreen::StartupModule()
{
	const bool IsServer = IsRunningDedicatedServer();
	const bool CanRender = FApp::CanEverRender();
	if (IsServer || GIsEditor || !CanRender)
	{
		return;
	}

	FPreLoadScreenManager* LoadingScreenManager = FPreLoadScreenManager::Get();
	if (LoadingScreenManager == nullptr)
	{
		return;
	}

	PreLoadingScreen = MakeShared<FCloverPreloadScreen>();
	PreLoadingScreen->Init();
	LoadingScreenManager->RegisterPreLoadScreen(PreLoadingScreen);
	LoadingScreenManager->OnPreLoadScreenManagerCleanUp.AddRaw(this, &FCloverStartupScreen::OnPreLoadScreenManagerCleanUp);
	UE_LOG(CloverStartupScreen, Warning, TEXT("CloverStartupScreen module has been loaded"));
}

void FCloverStartupScreen::ShutdownModule()
{
	UE_LOG(CloverStartupScreen, Warning, TEXT("CloverStartupScreen module has been unloaded"));
}

bool FCloverStartupScreen::IsGameModule() const
{
	return true;
}

void FCloverStartupScreen::OnPreLoadScreenManagerCleanUp()
{
	//Once the PreLoadScreenManager is cleaning up, we can get rid of all our resources too
	PreLoadingScreen.Reset();
	ShutdownModule();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCloverStartupScreen, CloverStartupScreen)