// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECloverCameraModeBlendType : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	SinIn,
	SinOut,
	SinInOut,
	ExpoIn,
	ExpoOut,
	ExpoInOut,
	CircularIn,
	CircularOut,
	CircularInOut,
	Count UMETA(Hidden)
};