// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Clover/CoreSystem/CloverGameSystem.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "CloverCameraSystem.generated.h"

class UCloverCameraComponent;

UCLASS()
class CLOVER_API UCloverCameraSystem : public UCloverGameSystem
{
	GENERATED_BODY()

public:
	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Clover|Camera")
	static UCloverCameraComponent* FindCameraComponent(const AActor* Actor);
};