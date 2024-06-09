// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverWidget.h"

#include "Editor/WidgetCompilerLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CloverWidget)

DEFINE_LOG_CATEGORY(LogCloverWidget);

#define LOCTEXT_NAMESPACE "Clover"

UCloverWidget::UCloverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

TOptional<FUIInputConfig> UCloverWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
		case ECloverWidgetInputMode::GameAndMenu:
			return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
		case ECloverWidgetInputMode::Game:
			return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
		case ECloverWidgetInputMode::Menu:
			return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		case ECloverWidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
	}
}

#if WITH_EDITOR

void UCloverWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);
	if (!GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UCloverWidget, BP_GetDesiredFocusTarget)))
	{
		if (GetParentNativeClass(GetClass()) == StaticClass())
		{
			CompileLog.Warning(
				LOCTEXT(
					"ValidateGetDesiredFocusTarget_Warning",
					"GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen."
				)
			);
		}
		else
		{
			//TODO - Note for now, because we can't guarantee it isn't implemented in a native subclass of this one.
			CompileLog.Note(
				LOCTEXT(
					"ValidateGetDesiredFocusTarget_Note",
					"GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen. If it was implemented in the native base class you can ignore this message."
				)
			);
		}
	}
}

#endif
#undef LOCTEXT_NAMESPACE