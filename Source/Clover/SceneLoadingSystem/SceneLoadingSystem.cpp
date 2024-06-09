// Copyright Clover Games, Inc. All Rights Reserved.


#include "SceneLoadingSystem.h"

void USceneLoadingSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnPreloadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnPostPreloadMap);
	const UGameInstance* LocalGameInstance = GetGameInstance();
	check(LocalGameInstance);
}

void USceneLoadingSystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

bool USceneLoadingSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UGameInstance* GameInstance = CastChecked<UGameInstance>(Outer);
	const bool bIsServerWorld = GameInstance->IsDedicatedServerInstance();
	return !bIsServerWorld;
}

void USceneLoadingSystem::Tick(float DeltaTime) {}

ETickableTickType USceneLoadingSystem::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

bool USceneLoadingSystem::IsTickable() const
{
	return !HasAnyFlags(RF_ClassDefaultObject);
}

TStatId USceneLoadingSystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(USceneLoadingSystem, STATGROUP_Tickables);
}

UWorld* USceneLoadingSystem::GetTickableGameObjectWorld() const
{
	const UGameInstance* Instance = GetGameInstance();
	if (Instance == nullptr)
	{
		return nullptr;
	}
	return Instance->GetWorld();
}

FString USceneLoadingSystem::GetDebugReasonForShowingOrHidingLoadingScreen() const
{
	return DebugReasonForShowingOrHidingLoadingScreen;
}

void USceneLoadingSystem::OnPreloadMap(const FWorldContext& WorldContext, const FString& MapName) {}
void USceneLoadingSystem::OnPostPreloadMap(UWorld* World) {}