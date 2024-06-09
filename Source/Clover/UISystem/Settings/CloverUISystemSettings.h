// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "CloverUISystemSettings.generated.h"

class UCloverRootWidget;
class UCloverWidget;
enum class ECloverWidgetName : uint8;

UCLASS(config=Clover, defaultconfig, meta=(DisplayName="UISystem"))
class CLOVER_API UCloverUISystemSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UCloverUISystemSettings();

	UPROPERTY(config, EditAnywhere, Category = "Widget")
	TSoftClassPtr<UCloverRootWidget> LayoutRootClass;

	UPROPERTY(config, EditAnyWhere, Category = "Widget")
	TMap<ECloverWidgetName, TSoftClassPtr<UCloverWidget>> WidgetCollection;

	UPROPERTY(config, EditAnywhere, Category = "Widget | LoadingScreen")
	TSoftClassPtr<UCloverWidget> LoadingScreenWidgetClass;

	// The z-order of the loading screen widget in the viewport stack
	UPROPERTY(config, EditAnywhere, Category="Widget | LoadingScreen")
	int32 LoadingScreenZOrder = 10000;

	UPROPERTY(config, EditAnywhere, Category="Widget | LoadingScreen", meta=(ForceUnits=s))
	float HoldLoadingScreenAdditionalSecs = 2.0f;

	// The interval in seconds beyond which the loading screen is considered permanently hung (if non-zero).
	UPROPERTY(config, EditAnywhere, Category="Widget | LoadingScreen", meta=(ForceUnits=s))
	float LoadingScreenHeartbeatHangDuration = 0.0f;

	// The interval in seconds between each log of what is keeping a loading screen up (if non-zero).
	UPROPERTY(config, EditAnywhere, Category="Widget | LoadingScreen", meta=(ForceUnits=s))
	float LogLoadingScreenHeartbeatInterval = 5.0f;

	// When true, the reason the loading screen is shown or hidden will be printed to the log every frame.
	UPROPERTY(Transient, EditAnywhere, Category="Widget | Debugging")
	bool LogLoadingScreenReasonEveryFrame = false;

	// Force the loading screen to be displayed (useful for debugging)
	UPROPERTY(Transient, EditAnywhere, Category="Widget | Debugging")
	bool ForceLoadingScreenVisible = false;

	// Should we apply the additional HoldLoadingScreenAdditionalSecs delay even in the editor
	// (useful when iterating on loading screens)
	UPROPERTY(Transient, EditAnywhere, Category="Widget | Debugging")
	bool HoldLoadingScreenAdditionalSecsEvenInEditor = false;

	// Should we apply the additional HoldLoadingScreenAdditionalSecs delay even in the editor
	// (useful when iterating on loading screens)
	UPROPERTY(config, EditAnywhere, Category="Widget | LoadingScreen")
	bool ForceTickLoadingScreenEvenInEditor = true;
};