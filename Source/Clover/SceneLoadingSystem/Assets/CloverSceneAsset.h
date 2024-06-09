// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "CloverSceneAsset.generated.h"

UCLASS()
class CLOVER_API UCloverSceneAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> SceneAsset;
};