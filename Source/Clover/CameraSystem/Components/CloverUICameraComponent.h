// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CloverUICameraComponent.generated.h"


class ACloverPlayerController;
class ACloverCameraManager;

UCLASS(Transient, Within=CloverCameraManager)
class CLOVER_API UCloverUICameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCloverUICameraComponent();
	virtual void InitializeComponent() override;
	bool IsSettingViewTarget() const;
	AActor* GetViewTarget() const;
	void SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams());
	bool NeedsToUpdateViewTarget() const;
	void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime);
	void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);

private:
	UPROPERTY(Transient)
	TObjectPtr<AActor> ViewTarget;

	UPROPERTY(Transient)
	bool bUpdatingViewTarget;

	void RegisterOnShowDebugInfo();
};