﻿// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "GameFramework/PlayerStart.h"
#include "CloverPlayerStart.generated.h"

class AController;
class UObject;

enum class ECloverPlayerStartLocationOccupancy
{
	Empty,
	Partial,
	Full
};

UCLASS(Config = Clover)
class CLOVER_API ACloverPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	explicit ACloverPlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const FGameplayTagContainer& GetGameplayTags()
	{
		return StartPointTags;
	}

	ECloverPlayerStartLocationOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const;

	/** Did this player start get claimed by a controller already? */
	bool IsClaimed() const;

	/** If this PlayerStart was not claimed, claim it for ClaimingController */
	bool TryClaim(AController* OccupyingController);

protected:
	/** Check if this PlayerStart is still claimed */
	void CheckUnclaimed();

	/** The controller that claimed this PlayerStart */
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	/** Interval in which we'll check if this player start is not colliding with anyone anymore */
	UPROPERTY(EditDefaultsOnly, Category = "Player Start Claiming")
	float ExpirationCheckInterval = 1.f;

	/** Tags to identify this player start */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer StartPointTags;

	/** Handle to track expiration recurring timer */
	FTimerHandle ExpirationTimerHandle;
};