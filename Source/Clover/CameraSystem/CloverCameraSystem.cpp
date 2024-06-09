// Copyright Clover Games, Inc. All Rights Reserved.

#include "CloverCameraSystem.h"
#include "Components/CloverCameraComponent.h"


UCloverCameraComponent* UCloverCameraSystem::FindCameraComponent(const AActor* Actor)
{
	return (Actor ? Actor->FindComponentByClass<UCloverCameraComponent>() : nullptr);
}