// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "ECloverWidgetName.generated.h"

UENUM(BlueprintType)
enum class ECloverWidgetName : uint8
{
	LoadingScreen,
	None,
};


UENUM(BlueprintType)
enum class ECloverWidgetLayer : uint8
{
	Top,
	Auto,
	Bottom,
	Model,
};