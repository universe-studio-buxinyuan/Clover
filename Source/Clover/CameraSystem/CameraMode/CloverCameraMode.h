// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "ECameraModeBlendType.h"
#include "FCameraModeData.h"
#include "Engine/World.h"
#include "CloverCameraMode.generated.h"

UCLASS(Abstract, NotBlueprintable)
class CLOVER_API UCloverCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UCloverCameraMode();

	void Evaluate(AActor* Target, float DeltaTime);
	void SetBlendWeightAndAlpha(const float Weight);

	FCloverCameraModeData& GetModeData();
	ECloverCameraModeBlendType GetBlendType();
	FVector GetLocationOffset();
	float GetFieldOfView();
	float GetViewPitchMin();
	float GetViewPitchMax();
	float GetBlendExponent();
	float GetBlendTime();
	float GetBlendWeight();
	float GetBlendAlpha();

	virtual void DrawDebug(UCanvas* Canvas) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	float LocationOffsetX;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	float LocationOffsetY;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	float LocationOffsetZ;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration | Blending")
	float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration | Blending")
	ECloverCameraModeBlendType BlendFunction;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration | Blending")
	float BlendExponent;

private:
	UPROPERTY(transient)
	uint32 bResetInterpolation : 1;

	FCloverCameraModeData ModeData;

	float BlendAlpha;
	float BlendWeight;

	FVector CalcPivotLocation(AActor* Target);
	FRotator CalcPivotRotation(AActor* Target);

	static FVector GetTargetLocation(AActor* Target);
};