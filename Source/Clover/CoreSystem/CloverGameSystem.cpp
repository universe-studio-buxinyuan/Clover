// Copyright Clover Games, Inc. All Rights Reserved.

#include "CloverGameSystem.h"
#include "Clover/Globals.h"


void UCloverGameSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogClover, Display, TEXT("[%s] initialize..."), *GetNameSafe(this))
}

void UCloverGameSystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UCloverGameSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}