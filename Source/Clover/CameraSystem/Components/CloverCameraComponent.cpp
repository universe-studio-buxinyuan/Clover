// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverCameraComponent.h"

#include "Clover/CameraSystem/CameraMode/CloverCameraModeCollection.h"
#include "Engine/Canvas.h"

UCloverCameraComponent::UCloverCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraModeCollection = nullptr;
}

AActor* UCloverCameraComponent::GetLookingActor() const
{
	return GetOwner();
}

void UCloverCameraComponent::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("CloverCameraComponent: %s"), *GetNameSafe(GetLookingActor())));

	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   Location: %s"), *GetComponentLocation().ToCompactString()));
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   Rotation: %s"), *GetComponentRotation().ToCompactString()));
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   FOV: %f"), FieldOfView));

	check(CameraModeCollection);
	CameraModeCollection->DrawDebug(Canvas);
}

void UCloverCameraComponent::SwitchCameraMode(const TSubclassOf<UCloverCameraMode>& CameraMode)
{
	if (CameraModeCollection == nullptr)
	{
		return;
	}

	CameraModeCollection->SwitchMode(CameraMode);
}

void UCloverCameraComponent::OnRegister()
{
	Super::OnRegister();
	if (!CameraModeCollection)
	{
		CameraModeCollection = NewObject<UCloverCameraModeCollection>(this);
		check(CameraModeCollection);
	}
}

void UCloverCameraComponent::GetCameraView(const float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);
	AActor* Target = GetLookingActor();
	const FCloverCameraModeData& CameraData = CameraModeCollection->Evaluate(Target, DeltaTime);
	SyncControlRotation(CameraData);
	SyncCameraMode(CameraData, DesiredView);
	if (IsXRHeadTrackedCamera())
	{
		// In XR much of the camera behavior above is irrellevant, but the post process settings are not.
		Super::GetCameraView(DeltaTime, DesiredView);
	}
}

void UCloverCameraComponent::SyncControlRotation(const FCloverCameraModeData& ModeData)
{
	AActor* TargetActor = GetLookingActor();
	if (TargetActor == nullptr)
	{
		return;
	}
	const APawn* TargetPawn = Cast<APawn>(TargetActor);
	if (TargetPawn == nullptr)
	{
		return;
	}
	if (APlayerController* PlayerController = TargetPawn->GetController<APlayerController>())
	{
		PlayerController->SetControlRotation(ModeData.ControlRotation);
	}
}

void UCloverCameraComponent::SyncCameraMode(const FCloverCameraModeData& Source, FMinimalViewInfo& Target)
{
	SetWorldLocationAndRotation(Source.Location, Source.Rotation);
	FieldOfView = Source.FieldOfView;
	Target.Location = Source.Location;
	Target.Rotation = Source.Rotation;
	Target.FOV = Source.FieldOfView;
	Target.OrthoWidth = OrthoWidth;
	Target.OrthoNearClipPlane = OrthoNearClipPlane;
	Target.OrthoFarClipPlane = OrthoFarClipPlane;
	Target.AspectRatio = AspectRatio;
	Target.bConstrainAspectRatio = bConstrainAspectRatio;
	Target.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	Target.ProjectionMode = ProjectionMode;
	Target.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		Target.PostProcessSettings = PostProcessSettings;
	}
}