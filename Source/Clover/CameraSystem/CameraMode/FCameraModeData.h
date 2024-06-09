// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FCloverCameraModeData
{
	FCloverCameraModeData();

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;

	void Blend(const FCloverCameraModeData& Other, float OtherWeight);
};