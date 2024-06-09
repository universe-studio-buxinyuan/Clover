// Copyright Clover Games, Inc. All Rights Reserved.
#include "CloverWidgetPolicy.h"

FRootViewportLayoutInfo::FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UCloverRootWidget* InRootLayout, const bool bIsInViewport): LocalPlayer(InLocalPlayer)
	, RootLayout(InRootLayout)
	, bAddedToViewport(bIsInViewport) {}

bool FRootViewportLayoutInfo::operator==(const ULocalPlayer* OtherLocalPlayer) const
{
	return LocalPlayer == OtherLocalPlayer;
}