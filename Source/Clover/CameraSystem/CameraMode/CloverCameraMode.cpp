// Copyright Clover Games, Inc. All Rights Reserved.

#include "CloverCameraMode.h"
#include "Clover/CameraSystem/CameraConstraint.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"

UCloverCameraMode::UCloverCameraMode()
{
	LocationOffsetX = 0.0f;
	LocationOffsetY = 0.0f;
	LocationOffsetZ = 0.0f;

	FieldOfView = CameraConstraint::DefaultFov;
	ViewPitchMin = CameraConstraint::DefaultPitchMin;
	ViewPitchMax = CameraConstraint::DefaultPitchMax;

	BlendTime = 0.5f;
	BlendFunction = ECloverCameraModeBlendType::EaseOut;
	BlendExponent = 4.0f;
	bResetInterpolation = 1;

	BlendWeight = 1.0f;
	BlendAlpha = 1.0f;
}

void UCloverCameraMode::Evaluate(AActor* Target, const float DeltaTime)
{
	const FVector Location = CalcPivotLocation(Target);
	const FRotator Rotation = CalcPivotRotation(Target);
	ModeData.Location = Location;
	ModeData.Rotation = Rotation;
	ModeData.ControlRotation = ModeData.Rotation;
	ModeData.FieldOfView = FieldOfView;
	if (BlendTime > 0.0f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}
	SetBlendWeightAndAlpha(DeltaTime);
}

void UCloverCameraMode::SetBlendWeightAndAlpha(const float Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);
	const float InvExponent = BlendExponent > 0.0f ? 1.0f / BlendExponent : 1.0f;
	switch (BlendFunction)
	{
		case ECloverCameraModeBlendType::Linear:
			BlendAlpha = BlendWeight;
			break;

		case ECloverCameraModeBlendType::EaseIn:
			BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, InvExponent);
			break;

		case ECloverCameraModeBlendType::EaseOut:
			BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, InvExponent);
			break;

		case ECloverCameraModeBlendType::EaseInOut:
			BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, InvExponent);
			break;

		case ECloverCameraModeBlendType::SinIn:
			BlendAlpha = FMath::InterpSinIn(0.0f, 1.0f, BlendWeight);
			break;

		case ECloverCameraModeBlendType::SinOut:
			BlendAlpha = FMath::InterpSinOut(0.0f, 1.0f, BlendWeight);
			break;

		default:
			checkf(false, TEXT("SetBlendWeight: Invalid BlendFunction [%d]\n"), static_cast<uint8>(BlendFunction));
			break;
	}
}

FCloverCameraModeData& UCloverCameraMode::GetModeData()
{
	return ModeData;
}


ECloverCameraModeBlendType UCloverCameraMode::GetBlendType()
{
	return BlendFunction;
}

FVector UCloverCameraMode::GetLocationOffset()
{
	return FVector(LocationOffsetX, LocationOffsetY, LocationOffsetZ);
}

float UCloverCameraMode::GetFieldOfView()
{
	return FieldOfView;
}

float UCloverCameraMode::GetViewPitchMin()
{
	return ViewPitchMin;
}

float UCloverCameraMode::GetViewPitchMax()
{
	return ViewPitchMax;
}

float UCloverCameraMode::GetBlendExponent()
{
	return BlendExponent;
}

float UCloverCameraMode::GetBlendTime()
{
	return BlendTime;
}

float UCloverCameraMode::GetBlendWeight()
{
	return BlendWeight;
}

float UCloverCameraMode::GetBlendAlpha()
{
	return BlendAlpha;
}

void UCloverCameraMode::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("		CloverCameraMode: %s (%f)"), *GetName(), BlendWeight));
}

FVector UCloverCameraMode::GetTargetLocation(AActor* Target)
{
	check(Target);
	const APawn* TargetPawn = Cast<APawn>(Target);
	if (TargetPawn == nullptr)
	{
		return Target->GetActorLocation();
	}
	const ACharacter* TargetCharacter = Cast<ACharacter>(Target);
	if (TargetCharacter == nullptr)
	{
		return TargetPawn->GetPawnViewLocation();
	}
	const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
	check(TargetCharacterCDO);
	const UCapsuleComponent* CapsuleComp = TargetCharacter->GetCapsuleComponent();
	check(CapsuleComp);
	const UCapsuleComponent* CapsuleCompCDO = TargetCharacterCDO->GetCapsuleComponent();
	check(CapsuleCompCDO);
	const float DefaultHalfHeight = CapsuleCompCDO->GetUnscaledCapsuleHalfHeight();
	const float ActualHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
	const float HeightAdjustment = DefaultHalfHeight - ActualHalfHeight + TargetCharacterCDO->BaseEyeHeight;
	return TargetCharacter->GetActorLocation() + (FVector::UpVector * HeightAdjustment);
}

FVector UCloverCameraMode::CalcPivotLocation(AActor* Target)
{
	check(Target);
	FVector Location = GetTargetLocation(Target);
	Location.X = Location.X + LocationOffsetX;
	Location.Y = Location.Y + LocationOffsetY;
	Location.Z = Location.Z + LocationOffsetZ;
	return Location;
}

FRotator UCloverCameraMode::CalcPivotRotation(AActor* Target)
{
	check(Target);
	FRotator Rotation = Target->GetActorRotation();
	if (const APawn* TargetPawn = Cast<APawn>(Target))
	{
		Rotation = TargetPawn->GetViewRotation();
	}
	Rotation.Pitch = FMath::ClampAngle(Rotation.Pitch, ViewPitchMin, ViewPitchMax);
	return Rotation;
}