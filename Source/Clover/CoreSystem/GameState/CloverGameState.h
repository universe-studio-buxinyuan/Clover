// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameState.h"
#include "CloverGameState.generated.h"

class UObject;

UCLASS(Blueprintable)
class CLOVER_API ACloverGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

UCLASS(Blueprintable)
class CLOVER_API ACloverGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void HandleMatchHasStarted() override;
};