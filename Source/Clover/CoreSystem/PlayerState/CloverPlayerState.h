﻿// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerState.h"
#include "CloverPlayerState.generated.h"

namespace EEndPlayReason
{
	enum Type : int;
}


class UObject;

UCLASS(Blueprintable)
class CLOVER_API ACloverPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;

protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;
};