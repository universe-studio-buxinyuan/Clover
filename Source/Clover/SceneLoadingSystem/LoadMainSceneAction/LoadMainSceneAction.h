// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Clover/SceneLoadingSystem/Base/LoadingAction.h"
#include "LoadMainSceneAction.generated.h"

UCLASS(BlueprintType)
class ULoadMainSceneAction : public ULoadingAction
{
	GENERATED_BODY()

public:
	ULoadMainSceneAction();

	virtual void OnStart() override;
	virtual bool IsFinished() override;
	virtual void OnFinish() override;
};