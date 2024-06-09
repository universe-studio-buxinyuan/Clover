// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FCameraModeData.h"
#include "UObject/Object.h"
#include "CloverCameraModeCollection.generated.h"

class UCloverCameraMode;
struct FCloverCameraModeData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCameraModeChange, TSubclassOf<UCloverCameraMode>);

UCLASS(Blueprintable)
class CLOVER_API UCloverCameraModeCollection : public UObject
{
	GENERATED_BODY()

public:
	UCloverCameraModeCollection();

	void SwitchMode(const TSubclassOf<UCloverCameraMode>& ModeClass);
	FOnCameraModeChange& OnModeChange();
	FCloverCameraModeData& Evaluate(AActor* Target, const float DeltaTime);
	void DrawDebug(UCanvas* Canvas) const;

private:
	FOnCameraModeChange OnCameraModeChange;
	FCloverCameraModeData EmptyModeData;

	UPROPERTY()
	TArray<TObjectPtr<UCloverCameraMode>> ModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UCloverCameraMode>> ModeStack;

	UCloverCameraMode* GetModeInstance(const TSubclassOf<UCloverCameraMode>& ModeClass);
};