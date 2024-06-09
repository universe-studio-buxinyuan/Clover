// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "MoviePlayer.h"
#include "Widgets/SCompoundWidget.h"

class SPreloadingScreenWidget final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPreloadingScreenWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		const bool IsFinished = GetMoviePlayer()->IsLoadingFinished();
		return IsFinished ? EVisibility::Collapsed : EVisibility::Visible;
	}

	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};