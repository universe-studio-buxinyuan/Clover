// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"
#include "CloverPawn.generated.h"

class UObject;

UCLASS(Blueprintable)
class CLOVER_API ACloverPawn : public APawn
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};