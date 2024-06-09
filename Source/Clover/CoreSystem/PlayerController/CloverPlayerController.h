// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CloverPlayerController.generated.h"

class UObject;
class UCloverLocalPlayer;

UCLASS(Blueprintable)
class CLOVER_API ACloverPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACloverPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void ReceivedPlayer() override;
};