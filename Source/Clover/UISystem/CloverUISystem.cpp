// Copyright Clover Games, Inc. All Rights Reserved.
#include "CloverUISystem.h"

#include "CommonInputSubsystem.h"
#include "Clover/CoreSystem/LocalPlayer/CloverLocalPlayer.h"
#include "Layer/CloverRootWidget.h"
#include "Layer/CloverWidgetPolicy.h"
#include "Settings/CloverUISystemSettings.h"

int32 UCloverUISystem::InputSuspensions = 0;

UCloverUISystem::UCloverUISystem() {}

void UCloverUISystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCloverUISystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UCloverUISystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		// Only create an instance if there is no override implementation defined elsewhere
		return ChildClasses.Num() == 0;
	}

	return false;
}

void UCloverUISystem::NotifyPlayerAdded(UCloverLocalPlayer* LocalPlayer)
{
	if (!(ensure(LocalPlayer)))
	{
		return;
	}
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this](UCloverLocalPlayer* LocalPlayer, ACloverPlayerController* PlayerController)
	{
		NotifyPlayerRemoved(LocalPlayer);
		CreateRootLayoutForPlayer(LocalPlayer);
	});
	CreateRootLayoutForPlayer(LocalPlayer);
}

void UCloverUISystem::NotifyPlayerRemoved(UCloverLocalPlayer* LocalPlayer)
{
	FRootViewportLayoutInfo* LayoutInfo = GetPlayerRootLayout(LocalPlayer);
	if (!LayoutInfo)
	{
		return;
	}

	RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
	LayoutInfo->bAddedToViewport = false;
	if (LocalMultiplayerInteractionMode != ELocalMultiplayerInteractionMode::SingleToggle || LocalPlayer->IsPrimaryPlayer())
	{
		return;
	}

	const TObjectPtr<UCloverRootWidget> RootWidget = LayoutInfo->RootLayout;
	if (!RootWidget || RootWidget->IsDormant())
	{
		return;
	}

	RootWidget->SetIsDormant(true);
	for (const FRootViewportLayoutInfo& RootLayoutInfo : RootViewportLayouts)
	{
		if (RootLayoutInfo.LocalPlayer->IsPrimaryPlayer())
		{
			if (UCloverRootWidget* PrimaryRootLayout = RootLayoutInfo.RootLayout)
			{
				PrimaryRootLayout->SetIsDormant(false);
			}
		}
	}
}

void UCloverUISystem::NotifyPlayerDestroyed(UCloverLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);
	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx == INDEX_NONE)
	{
		return;
	}

	UCloverRootWidget* Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
	RootViewportLayouts.RemoveAt(LayoutInfoIdx);
	RemoveLayoutFromViewport(LocalPlayer, Layout);
	OnRootLayoutReleased(LocalPlayer, Layout);
}

ECommonInputType UCloverUISystem::GetOwningPlayerInputType(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType();
		}
	}

	return ECommonInputType::Count;
}

bool UCloverUISystem::IsOwningPlayerUsingTouch(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType() == ECommonInputType::Touch;
		}
	}
	return false;
}

bool UCloverUISystem::IsOwningPlayerUsingGamepad(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad;
		}
	}
	return false;
}

UCloverWidget* UCloverUISystem::OpenWidget(ULocalPlayer* LocalPlayer, const ECloverWidgetLayer LayerName, const ECloverWidgetName WidgetName)
{
	if (!ensure(LocalPlayer))
	{
		return nullptr;
	}

	UCloverUISystem* UISystem = LocalPlayer->GetGameInstance()->GetSubsystem<UCloverUISystem>();
	if (!UISystem)
	{
		return nullptr;
	}

	return UISystem->InternalOpenWidget(LocalPlayer, LayerName, WidgetName);
}

void UCloverUISystem::OpenWidgetAsync(ULocalPlayer* LocalPlayer, const ECloverWidgetLayer LayerName, const ECloverWidgetName WidgetName, bool bSuspendInputUntilComplete)
{
	if (!ensure(LocalPlayer))
	{
		return;
	}

	UCloverUISystem* UISystem = LocalPlayer->GetGameInstance()->GetSubsystem<UCloverUISystem>();
	if (!UISystem)
	{
		return;
	}

	UISystem->InternalOpenWidgetAsync(LocalPlayer, LayerName, WidgetName, bSuspendInputUntilComplete);
}

void UCloverUISystem::PopContentFromLayer(UCloverWidget* Widget) {}

ULocalPlayer* UCloverUISystem::GetLocalPlayerFromController(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		return Cast<ULocalPlayer>(PlayerController->Player);
	}

	return nullptr;
}

FName UCloverUISystem::SuspendInputForPlayer(APlayerController* PlayerController, const FName SuspendReason)
{
	return SuspendInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, SuspendReason);
}

FName UCloverUISystem::SuspendInputForPlayer(const ULocalPlayer* LocalPlayer, const FName SuspendReason)
{
	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		InputSuspensions++;
		FName SuspendToken = SuspendReason;
		SuspendToken.SetNumber(InputSuspensions);

		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, true);

		return SuspendToken;
	}

	return NAME_None;
}

void UCloverUISystem::ResumeInputForPlayer(APlayerController* PlayerController, const FName SuspendToken)
{
	ResumeInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, SuspendToken);
}

void UCloverUISystem::ResumeInputForPlayer(const ULocalPlayer* LocalPlayer, const FName SuspendToken)
{
	if (SuspendToken == NAME_None)
	{
		return;
	}

	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, false);
	}
}

void UCloverUISystem::CreateLayoutWidget(UCloverLocalPlayer* LocalPlayer)
{
	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld());
	if (PlayerController == nullptr)
	{
		return;
	}

	const TSubclassOf<UCloverRootWidget> LayoutWidgetClass = GetLayoutWidgetClass();
	if (!(ensure(LayoutWidgetClass)))
	{
		return;
	}

	if (!LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract))
	{
		return;
	}

	UCloverRootWidget* NewLayoutObject = CreateWidget<UCloverRootWidget>(PlayerController, LayoutWidgetClass);
	EmplaceLocalPlayerLayout(LocalPlayer, NewLayoutObject);
}

TSubclassOf<UCloverRootWidget> UCloverUISystem::GetLayoutWidgetClass()
{
	const UCloverUISystemSettings* Settings = GetDefault<UCloverUISystemSettings>();
	if (Settings->LayoutRootClass.IsNull())
	{
		UE_LOG(LogCloverWidget, Error, TEXT("RootWidget SoftClass Reference is invalid "));
		return nullptr;
	}
	TSubclassOf<UCloverRootWidget> LayoutClass = Settings->LayoutRootClass.LoadSynchronous();
	return LayoutClass;
}

UCloverWidget* UCloverUISystem::InternalOpenWidget(ULocalPlayer* LocalPlayer, const ECloverWidgetLayer LayerName, const ECloverWidgetName WidgetName)
{
	if (!ensure(LocalPlayer))
	{
		return nullptr;
	}

	const TSubclassOf<UCloverWidget> WidgetClass = GetWidgetClass(WidgetName);
	if (!ensure(WidgetClass != nullptr))
	{
		return nullptr;
	}

	UCloverLocalPlayer* Player = CastChecked<UCloverLocalPlayer>(LocalPlayer);
	UCloverRootWidget* RootWidget = GetPlayerRootWidget(Player);
	if (!RootWidget)
	{
		return nullptr;
	}

	return RootWidget->PushWidgetToLayerStack(LayerName, WidgetClass);
}

void UCloverUISystem::InternalOpenWidgetAsync(ULocalPlayer* LocalPlayer, const ECloverWidgetLayer LayerName, const ECloverWidgetName WidgetName, const bool bSuspendInputUntilComplete)
{
	if (!ensure(LocalPlayer))
	{
		return;
	}

	const TSoftClassPtr<UCloverWidget> WidgetClass = GetWidgetClassSoftPtr(WidgetName);
	if (WidgetClass.IsNull())
	{
		return;
	}

	UCloverLocalPlayer* Player = CastChecked<UCloverLocalPlayer>(LocalPlayer);
	UCloverRootWidget* RootWidget = GetPlayerRootWidget(Player);
	if (!RootWidget)
	{
		return;
	}

	RootWidget->PushWidgetToLayerStackAsync(LayerName, WidgetClass, bSuspendInputUntilComplete);
}

void UCloverUISystem::CreateRootLayoutForPlayer(UCloverLocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = GetPlayerRootLayout(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UCloverUISystem::AddLayoutToViewport(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget)
{
	UE_LOG(LogCloverWidget, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(RootWidget));

	RootWidget->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	RootWidget->AddToPlayerScreen(1000);

	OnRootLayoutAddedToViewport(LocalPlayer, RootWidget);
}

void UCloverUISystem::RemoveLayoutFromViewport(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget)
{
	const TWeakPtr<SWidget> LayoutSlateWidget = RootWidget->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		UE_LOG(
			LogCloverWidget, Log,
			TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"),
			*GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(RootWidget)
		);

		RootWidget->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(
				LogCloverWidget, Log,
				TEXT("Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."),
				*GetNameSafe(LocalPlayer), *GetNameSafe(RootWidget));
		}

		OnRootLayoutRemoveFromViewport(LocalPlayer, RootWidget);
	}
}

void UCloverUISystem::OnRootLayoutAddedToViewport(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget)
{
#if WITH_EDITOR
	if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
	{
		// So our controller will work in PIE without needing to click in the viewport
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}
#endif
	OnRootWidgetAdd.Broadcast(LocalPlayer, RootWidget);
}

void UCloverUISystem::OnRootLayoutRemoveFromViewport(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget)
{
	OnRootWidgetRemove.Broadcast(LocalPlayer, RootWidget);
}

void UCloverUISystem::OnRootLayoutReleased(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget)
{
	OnRootWidgetRelease.Broadcast(LocalPlayer, RootWidget);
}

void UCloverUISystem::EmplaceLocalPlayerLayout(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* LayoutWidget)
{
	RootViewportLayouts.Emplace(LocalPlayer, LayoutWidget, true);
}

FRootViewportLayoutInfo* UCloverUISystem::GetPlayerRootLayout(UCloverLocalPlayer* LocalPlayer)
{
	return RootViewportLayouts.FindByKey(LocalPlayer);
}

UCloverRootWidget* UCloverUISystem::GetPlayerRootWidget(UCloverLocalPlayer* LocalPlayer)
{
	const FRootViewportLayoutInfo* LayoutInfo = GetPlayerRootLayout(LocalPlayer);
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

TSubclassOf<UCloverWidget> UCloverUISystem::GetWidgetClass(const ECloverWidgetName WidgetName)
{
	const TSoftClassPtr<UCloverWidget> WidgetClassPtr = GetWidgetClassSoftPtr(WidgetName);
	return WidgetClassPtr.LoadSynchronous();
}

TSoftClassPtr<UCloverWidget> UCloverUISystem::GetWidgetClassSoftPtr(const ECloverWidgetName WidgetName)
{
	const UCloverUISystemSettings* Settings = GetDefault<UCloverUISystemSettings>();
	const TSoftClassPtr<UCloverWidget> WidgetClassPtr = Settings->WidgetCollection.FindRef(WidgetName);
	return WidgetClassPtr;
}


ELocalMultiplayerInteractionMode UCloverUISystem::GetLocalMultiplayerInteractionMode() const
{
	return LocalMultiplayerInteractionMode;
}