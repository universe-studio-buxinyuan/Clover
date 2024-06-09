// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "CloverWidget.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogCloverWidget, Log, All);

UENUM(BlueprintType)
enum class ECloverWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

UCLASS(Abstract, Blueprintable)
class CLOVER_API UCloverWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	explicit UCloverWidget(const FObjectInitializer& ObjectInitializer);
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, IWidgetCompilerLog& CompileLog) const override;
#endif

protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ECloverWidgetInputMode InputConfig = ECloverWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};