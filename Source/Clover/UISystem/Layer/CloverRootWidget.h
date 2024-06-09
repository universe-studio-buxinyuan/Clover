// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "Clover/UISystem/CloverUISystem.h"
#include "Clover/UISystem/Base/CloverWidget.h"
#include "Engine/AssetManager.h"
#include "Clover/UISystem/Layer/CloverWidgetContainer.h"
#include "CloverRootWidget.generated.h"

class UCommonActivatableWidgetContainerBase;
struct FStreamableHandle;
enum class ECloverWidgetName : uint8;
enum class ECloverWidgetLayer : uint8;
class UCloverWidgetContainer;

enum class EAsyncWidgetLayerState : uint8
{
	Canceled,
	Initialize,
	AfterPush
};

UCLASS(Abstract)
class CLOVER_API UCloverRootWidget : public UCloverWidget
{
	GENERATED_BODY()

public:
	explicit UCloverRootWidget(const FObjectInitializer& Initializer);

	/** A dormant root layout is collapsed and responds only to persistent actions registered by the owning player */
	void SetIsDormant(bool InDormant);

	bool IsDormant() const;

	template <typename TWidget = UCloverWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(
		const ECloverWidgetLayer LayerName, TSoftClassPtr<UCloverWidget> WidgetClass,
		const bool bSuspendInputUntilComplete, TFunction<void(EAsyncWidgetLayerState, TWidget*)> StateFunc
	);

	template <typename TWidget = UCloverWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(
		const ECloverWidgetLayer LayerName, TSoftClassPtr<UCloverWidget> WidgetClass, bool bSuspendInputUntilComplete
	);

	template <typename TWidget = UCloverWidget>
	TWidget* PushWidgetToLayerStack(const ECloverWidgetLayer LayerName, UClass* WidgetClass);


	template <typename TWidget = UCloverWidget>
	TWidget* PushWidgetToLayerStack(const ECloverWidgetLayer LayerName, UClass* WidgetClass, TFunctionRef<void(TWidget&)> InitInstanceFunc);

	void FindAndRemoveWidgetFromLayer(UCloverWidget* Widget);
	UCloverWidgetContainer* GetLayerWidget(ECloverWidgetLayer LayerName);

protected:
	virtual void OnIsDormantChanged();
	void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget, bool bIsTransitioning);

	UFUNCTION(BlueprintCallable, Category="Clover| Layer")
	void RegisterLayer(ECloverWidgetLayer Layer, UCloverWidgetContainer* LayerWidget);

private:
	bool bIsDormant = false;

	// Lets us keep track of all suspended input tokens so that multiple async UIs can be loading and we correctly suspend
	// for the duration of all of them.
	TArray<FName> SuspendInputTokens;

	// The registered layers for the primary layout.
	UPROPERTY(Transient)
	TMap<ECloverWidgetLayer, TObjectPtr<UCloverWidgetContainer>> Layers;
};

template <typename TWidget>
TSharedPtr<FStreamableHandle> UCloverRootWidget::PushWidgetToLayerStackAsync(
	const ECloverWidgetLayer LayerName, TSoftClassPtr<UCloverWidget> WidgetClass,
	const bool bSuspendInputUntilComplete, TFunction<void(EAsyncWidgetLayerState, TWidget*)> StateFunc)
{
	static_assert(TIsDerivedFrom<TWidget, UCloverWidget>::IsDerived, "Only UCloverWidget can be used here");

	static FName Name_PushingWidgetToLayer("PushingWidgetToLayer");
	const FName SuspendInputToken = bSuspendInputUntilComplete ? UCloverUISystem::SuspendInputForPlayer(GetOwningPlayer(), Name_PushingWidgetToLayer) : NAME_None;

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
	TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(
		WidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(
			this,
			[this, LayerName, WidgetClass, StateFunc, SuspendInputToken]
			{
				UCloverUISystem::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);
				TWidget* Widget = PushWidgetToLayerStack<TWidget>(
					LayerName, WidgetClass.Get(), [StateFunc](TWidget& WidgetToInit)
					{
						StateFunc(EAsyncWidgetLayerState::Initialize, &WidgetToInit);
					});

				StateFunc(EAsyncWidgetLayerState::AfterPush, Widget);
			})
	);

	// Setup a cancel delegate so that we can resume input if this handler is canceled.
	StreamingHandle->BindCancelDelegate(
		FStreamableDelegate::CreateWeakLambda(
			this,
			[this, StateFunc, SuspendInputToken]
			{
				UCloverUISystem::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);
				StateFunc(EAsyncWidgetLayerState::Canceled, nullptr);
			})
	);

	return StreamingHandle;
}

template <typename TWidget>
TSharedPtr<FStreamableHandle> UCloverRootWidget::PushWidgetToLayerStackAsync(const ECloverWidgetLayer LayerName, TSoftClassPtr<UCloverWidget> WidgetClass, bool bSuspendInputUntilComplete)
{
	return PushWidgetToLayerStackAsync<TWidget>(LayerName, WidgetClass, bSuspendInputUntilComplete, [](EAsyncWidgetLayerState, TWidget*) {});
}

template <typename TWidget>
TWidget* UCloverRootWidget::PushWidgetToLayerStack(const ECloverWidgetLayer LayerName, UClass* WidgetClass)
{
	return PushWidgetToLayerStack<TWidget>(LayerName, WidgetClass, [](TWidget&) {});
}

template <typename TWidget>
TWidget* UCloverRootWidget::PushWidgetToLayerStack(const ECloverWidgetLayer LayerName, UClass* WidgetClass, TFunctionRef<void(TWidget&)> InitInstanceFunc)
{
	static_assert(TIsDerivedFrom<TWidget, UCloverWidget>::IsDerived, "Only UCloverWidget can be used here");

	if (UCloverWidgetContainer* Layer = GetLayerWidget(LayerName))
	{
		return Layer->AddWidget<TWidget>(WidgetClass, InitInstanceFunc);
	}

	return nullptr;
}