// Copyright Clover Games, Inc. All Rights Reserved.
#include "CloverGamePlayerController.h"
#include "Clover/CameraSystem/CameraManager/CloverCameraManager.h"

ACloverGamePlayerController::ACloverGamePlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ACloverCameraManager::StaticClass();
}