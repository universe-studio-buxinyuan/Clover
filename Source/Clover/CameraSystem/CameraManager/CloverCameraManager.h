// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "CloverCameraManager.generated.h"

class UCloverUICameraComponent;

UCLASS(notplaceable)
class CLOVER_API ACloverCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	explicit ACloverCameraManager(const FObjectInitializer& ObjectInitializer);

	UCloverUICameraComponent* GetUICameraComponent() const;

protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Yl, float& YPos) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UCloverUICameraComponent> UICamera;
};