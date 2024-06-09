// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"

void ACloverPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ACloverPlayerState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ACloverPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void ACloverPlayerState::Reset()
{
	Super::Reset();
	TArray<UPlayerStateComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (UPlayerStateComponent* Component : ModularComponents)
	{
		Component->Reset();
	}
}

void ACloverPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	TInlineComponentArray<UPlayerStateComponent*> PlayerStateComponents;
	GetComponents(PlayerStateComponents);
	for (UPlayerStateComponent* SourcePSComp : PlayerStateComponents)
	{
		UObjectBase* TargetUnit = FindObjectWithOuter(PlayerState, SourcePSComp->GetClass(), SourcePSComp->GetFName());
		UObject* TargetObject = static_cast<UObject*>(TargetUnit);
		if (UPlayerStateComponent* TargetComp = Cast<UPlayerStateComponent>(TargetObject))
		{
			SourcePSComp->CopyProperties(TargetComp);
		}
	}
}