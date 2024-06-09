// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverRootWidget.h"

#include "CloverWidgetContainer.h"
UCloverRootWidget::UCloverRootWidget(const FObjectInitializer& Initializer) : Super(Initializer) {}

void UCloverRootWidget::SetIsDormant(const bool InDormant)
{
	if (bIsDormant != InDormant)
	{
		const ULocalPlayer* Lp = GetOwningLocalPlayer();
		const int32 PlayerId = Lp ? Lp->GetControllerId() : -1;
		const TCHAR* OldDormancyStr = bIsDormant ? TEXT("Dormant") : TEXT("Not-Dormant");
		const TCHAR* NewDormancyStr = InDormant ? TEXT("Dormant") : TEXT("Not-Dormant");
		const TCHAR* PrimaryPlayerStr = Lp && Lp->IsPrimaryPlayer() ? TEXT("[Primary]") : TEXT("[Non-Primary]");
		UE_LOG(LogCloverWidget, Display, TEXT("%s RootWidget Dormancy changed for [%d] from [%s] to [%s]"), PrimaryPlayerStr, PlayerId, OldDormancyStr, NewDormancyStr);

		bIsDormant = InDormant;
		OnIsDormantChanged();
	}
}

bool UCloverRootWidget::IsDormant() const
{
	return bIsDormant;
}

void UCloverRootWidget::FindAndRemoveWidgetFromLayer(UCloverWidget* Widget)
{
	for (const auto& LayerKvp : Layers)
	{
		LayerKvp.Value->RemoveWidget(*Widget);
	}
}

UCloverWidgetContainer* UCloverRootWidget::GetLayerWidget(const ECloverWidgetLayer LayerName)
{
	return Layers.FindRef(LayerName);
}

void UCloverRootWidget::OnIsDormantChanged() {}

void UCloverRootWidget::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget, const bool bIsTransitioning)
{
	if (bIsTransitioning)
	{
		const FName SuspendToken = UCloverUISystem::SuspendInputForPlayer(GetOwningLocalPlayer(), TEXT("GlobalStackTransion"));
		SuspendInputTokens.Add(SuspendToken);
	}
	else
	{
		if (ensure(SuspendInputTokens.Num() > 0))
		{
			const FName SuspendToken = SuspendInputTokens.Pop();
			UCloverUISystem::ResumeInputForPlayer(GetOwningLocalPlayer(), SuspendToken);
		}
	}
}

void UCloverRootWidget::RegisterLayer(const ECloverWidgetLayer Layer, UCloverWidgetContainer* LayerWidget)
{
	if (!IsDesignTime())
	{
		LayerWidget->OnTransitioningChanged.AddUObject(this, &UCloverRootWidget::OnWidgetStackTransitioning);
		LayerWidget->SetTransitionDuration(0.0);
		Layers.Add(Layer, LayerWidget);
	}
}