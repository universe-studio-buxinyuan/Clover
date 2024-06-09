// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "Clover/CoreSystem/PlayerController/CloverPlayerController.h"
#include "CloverGamePlayerController.generated.h"

UCLASS()
class CLOVER_API ACloverGamePlayerController : public ACloverPlayerController
{
	GENERATED_BODY()

public:
	explicit ACloverGamePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};