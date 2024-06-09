// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CloverWidgetContainer.generated.h"

UCLASS(meta=(DisableNativeTick))
class CLOVER_API UCloverWidgetContainer : public UCommonActivatableWidgetContainerBase
{
	GENERATED_BODY()

public:
	explicit UCloverWidgetContainer(const FObjectInitializer& Initializer);
};

UCLASS()
class CLOVER_API UCloverWidgetStackContainer : public UCloverWidgetContainer
{
	GENERATED_BODY()
};

UCLASS()
class CLOVER_API UCloverWidgetQueueContainer : public UCloverWidgetContainer
{
	GENERATED_BODY()
};