// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "CloverCameraComponent.generated.h"

class UCloverCameraMode;
struct FCloverCameraModeData;
class USpringArmComponent;
class UCloverCameraModeCollection;

UCLASS()
class CLOVER_API UCloverCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	explicit UCloverCameraComponent(const FObjectInitializer& ObjectInitializer);
	virtual AActor* GetLookingActor() const;
	virtual void DrawDebug(UCanvas* Canvas) const;
	void SwitchCameraMode(const TSubclassOf<UCloverCameraMode>& CameraMode);

protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

private:
	UPROPERTY()
	TObjectPtr<UCloverCameraModeCollection> CameraModeCollection;

	void SyncControlRotation(const FCloverCameraModeData& ModeData);
	void SyncCameraMode(const FCloverCameraModeData& Source, FMinimalViewInfo& Target);
};