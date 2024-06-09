// Copyright Clover Games, Inc. All Rights Reserved.
#include "SPreloadingScreenWidget.h"

void SPreloadingScreenWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		.BorderBackgroundColor(FLinearColor::Black)
		.Padding(0)
	];
	// static const FName LoadingScreenName(TEXT("/Game/UI/T_CloverLogo.T_CloverLogo"));
	// FLoadingScreenBrush* BrushInstance = new FLoadingScreenBrush(LoadingScreenName, FVector2D(1920, 1680));
	// LoadingScreenBrush = MakeShareable(BrushInstance);
	//
	// FSlateBrush* BgBrush = new FSlateBrush();
	// BgBrush->TintColor = FLinearColor(0.034f, 0.034f, 0.034f, 1.0f);
	//
	// ChildSlot
	// [
	// 	SNew(SOverlay)
	// 	+ SOverlay::Slot()
	// 	.HAlign(HAlign_Fill)
	// 	.VAlign(VAlign_Fill)
	// 	[
	// 		SNew(SBorder)
	// 		.BorderImage(BgBrush)
	// 	]
	// 	+ SOverlay::Slot()
	// 	.HAlign(HAlign_Center)
	// 	.VAlign(VAlign_Center)
	// 	[
	// 		SNew(SImage)
	// 		.Image(LoadingScreenBrush.Get())
	// 	]
	// 	+ SOverlay::Slot()
	// 	.HAlign(HAlign_Fill)
	// 	.VAlign(VAlign_Fill)
	// 	[
	// 		SNew(SVerticalBox)
	// 		+ SVerticalBox::Slot()
	// 		.VAlign(VAlign_Bottom)
	// 		.HAlign(HAlign_Right)
	// 		.Padding(FMargin(10.0f))
	// 		[
	// 			SNew(SThrobber)
	// 			.Visibility(this, &SPreloadingScreenWidget::GetLoadIndicatorVisibility)
	// 		]
	// 	]
	// ];
}