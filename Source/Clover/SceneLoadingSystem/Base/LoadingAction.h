// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LoadingAction.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLoadingActionProcess, FString, float);

UCLASS(Abstract)
class ULoadingAction : public UObject
{
	GENERATED_BODY()

public:
	ULoadingAction() = default;

	void Start()
	{
		OnStart();
	}

	virtual void OnStart() {}

	virtual bool IsFinished()
	{
		return true;
	}

	void Finish()
	{
		OnFinish();
	}

	virtual void OnFinish() {}
};