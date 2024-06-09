// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverUICameraComponent.h"
#include "Clover/CameraSystem/CameraManager/CloverCameraManager.h"
#include "GameFramework/HUD.h"


UCloverUICameraComponent::UCloverUICameraComponent()
{
	bWantsInitializeComponent = true;
	RegisterOnShowDebugInfo();
}

void UCloverUICameraComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

bool UCloverUICameraComponent::IsSettingViewTarget() const
{
	return bUpdatingViewTarget;
}

AActor* UCloverUICameraComponent::GetViewTarget() const
{
	return ViewTarget;
}

void UCloverUICameraComponent::SetViewTarget(AActor* InViewTarget, const FViewTargetTransitionParams TransitionParams)
{
	TGuardValue UpdatingViewTargetGuard(bUpdatingViewTarget, true);
	ViewTarget = InViewTarget;
	CastChecked<ACloverCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UCloverUICameraComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UCloverUICameraComponent::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) {}

void UCloverUICameraComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos) {}

void UCloverUICameraComponent::RegisterOnShowDebugInfo()
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && !IsRunningDedicatedServer())
	{
		AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
	}
}