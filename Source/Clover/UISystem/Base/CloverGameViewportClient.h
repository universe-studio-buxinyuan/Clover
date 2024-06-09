// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CommonGameViewportClient.h"
#include "CloverGameViewportClient.generated.h"

UCLASS(BlueprintType)
class CLOVER_API UCloverGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	UCloverGameViewportClient();

	virtual void Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};