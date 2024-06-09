// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "CloverLocalPlayer.generated.h"

class ACloverPlayerController;
/**
 *
 */
UCLASS(config=Clover, transient)
class CLOVER_API UCloverLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	UCloverLocalPlayer();

	virtual bool GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const override;

	bool IsPlayerViewEnabled() const;

	void SetIsPlayerViewEnabled(const bool bInIsPlayerViewEnabled);

	bool bIsPlayerViewEnabled = true;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UCloverLocalPlayer* LocalPlayer, ACloverPlayerController* PlayerController);
	FPlayerControllerSetDelegate OnPlayerControllerSet;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, UCloverLocalPlayer* LocalPlayer, APlayerState* PlayerState);
	FPlayerStateSetDelegate OnPlayerStateSet;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPawnSetDelegate, UCloverLocalPlayer* LocalPlayer, APawn* Pawn);
	FPlayerPawnSetDelegate OnPlayerPawnSet;
};