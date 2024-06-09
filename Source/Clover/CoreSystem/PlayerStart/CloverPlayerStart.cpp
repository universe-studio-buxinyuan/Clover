// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverPlayerStart.h"
#include "GameFramework/GameModeBase.h"


ACloverPlayerStart::ACloverPlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

ECloverPlayerStartLocationOccupancy ACloverPlayerStart::GetLocationOccupancy(AController* const ControllerPawnToFit) const
{
	if (!HasAuthority())
	{
		return ECloverPlayerStartLocationOccupancy::Full;
	}

	UWorld* const World = GetWorld();
	if (!World)
	{
		return ECloverPlayerStartLocationOccupancy::Full;
	}
	AGameModeBase* AuthGameMode = World->GetAuthGameMode();
	if (!AuthGameMode)
	{
		return ECloverPlayerStartLocationOccupancy::Full;
	}

	const TSubclassOf<APawn> PawnClass = AuthGameMode->GetDefaultPawnClassForController(ControllerPawnToFit);
	const APawn* const PawnToFit = PawnClass ? GetDefault<APawn>(PawnClass) : nullptr;

	FVector ActorLocation = GetActorLocation();
	const FRotator ActorRotation = GetActorRotation();

	if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation, nullptr))
	{
		return ECloverPlayerStartLocationOccupancy::Empty;
	}

	if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
	{
		return ECloverPlayerStartLocationOccupancy::Partial;
	}

	return ECloverPlayerStartLocationOccupancy::Full;
}

bool ACloverPlayerStart::IsClaimed() const
{
	return ClaimingController != nullptr;
}

bool ACloverPlayerStart::TryClaim(AController* OccupyingController)
{
	if (OccupyingController == nullptr)
	{
		return false;
	}

	if (!IsClaimed())
	{
		return false;
	}

	ClaimingController = OccupyingController;
	if (const UWorld* World = GetWorld())
	{
		const auto UpdateFunction = FTimerDelegate::CreateUObject(this, &ACloverPlayerStart::CheckUnclaimed);
		World->GetTimerManager().SetTimer(ExpirationTimerHandle, UpdateFunction, ExpirationCheckInterval, true);
	}
	return true;
}

void ACloverPlayerStart::CheckUnclaimed()
{
	if (ClaimingController == nullptr)
	{
		return;
	}

	if (ClaimingController->GetPawn() == nullptr)
	{
		return;
	}

	if (GetLocationOccupancy(ClaimingController) != ECloverPlayerStartLocationOccupancy::Empty)
	{
		return;
	}

	ClaimingController = nullptr;
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ExpirationTimerHandle);
	}
}