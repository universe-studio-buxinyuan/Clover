// Copyright Clover Games, Inc. All Rights Reserved.


#include "FCameraModeData.h"

#include "Clover/CameraSystem/CameraConstraint.h"


FCloverCameraModeData::FCloverCameraModeData():
	Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(CameraConstraint::DefaultFov) {}

void FCloverCameraModeData::Blend(const FCloverCameraModeData& Other, const float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}

	if (OtherWeight >= 1.0f)
	{
		*this = Other;
		return;
	}

	Location = FMath::Lerp(Location, Other.Location, OtherWeight);
	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + OtherWeight * DeltaRotation;
	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + OtherWeight * DeltaControlRotation;
	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}