// Copyright Clover Games, Inc. All Rights Reserved.

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
	ACloverPlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};