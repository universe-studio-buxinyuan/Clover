// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverCameraManager.h"

#include "Clover/CameraSystem/CameraConstraint.h"
#include "Clover/CameraSystem/CloverCameraSystem.h"
#include "Clover/CameraSystem/Components/CloverCameraComponent.h"
#include "Clover/CameraSystem/Components/CloverUICameraComponent.h"
#include "Engine/Canvas.h"

class UCloverCameraComponent;
static FName UICameraComponentName(TEXT("UICamera"));

ACloverCameraManager::ACloverCameraManager(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	DefaultFOV = CameraConstraint::DefaultFov;
	ViewPitchMin = CameraConstraint::DefaultPitchMin;
	ViewPitchMax = CameraConstraint::DefaultPitchMax;
	UICamera = CreateDefaultSubobject<UCloverUICameraComponent>(UICameraComponentName);
}

UCloverUICameraComponent* ACloverCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void ACloverCameraManager::UpdateViewTarget(FTViewTarget& OutVT, const float DeltaTime)
{
	if (UICamera)
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void ACloverCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Yl, float& YPos)
{
	check(Canvas);
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;
	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("CloverCameraManager: %s"), *GetNameSafe(this)));
	Super::DisplayDebug(Canvas, DebugDisplay, Yl, YPos);
	if (PCOwner == nullptr)
	{
		return;
	}

	const APawn* Pawn = PCOwner->GetPawn();
	if (Pawn == nullptr)
	{
		return;
	}

	const UCloverCameraComponent* CameraComponent = UCloverCameraSystem::FindCameraComponent(Pawn);
	if (CameraComponent == nullptr)
	{
		return;
	}
	CameraComponent->DrawDebug(Canvas);
}