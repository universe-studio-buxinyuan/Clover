// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverLocalPlayer.h"
UCloverLocalPlayer::UCloverLocalPlayer() {}

bool UCloverLocalPlayer::GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const
{
	if (!bIsPlayerViewEnabled)
	{
		return false;
	}

	return Super::GetProjectionData(Viewport, ProjectionData, StereoViewIndex);
}

bool UCloverLocalPlayer::IsPlayerViewEnabled() const
{
	return bIsPlayerViewEnabled;
}

void UCloverLocalPlayer::SetIsPlayerViewEnabled(const bool bInIsPlayerViewEnabled)
{
	bIsPlayerViewEnabled = bInIsPlayerViewEnabled;
}