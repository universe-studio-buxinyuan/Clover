// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once
#include "CommonInputTypeEnum.h"
#include "ECloverWidgetName.h"
#include "Clover/CoreSystem/CloverGameSystem.h"
#include "Layer/CloverWidgetPolicy.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CloverUISystem.generated.h"

enum class ECloverWidgetName : uint8;
class UCloverWidget;
struct FRootViewportLayoutInfo;
class UCloverWidgetPolicy;
class UCloverRootWidget;
class UCloverLocalPlayer;

DECLARE_DELEGATE_OneParam(FOnCloverWidgetOpen, TObjectPtr<UCloverWidget>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRootWidgetAdd, UCloverLocalPlayer*, UCloverRootWidget*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRootWidgetRemove, UCloverLocalPlayer*, UCloverRootWidget*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRootWidgetRelease, UCloverLocalPlayer*, UCloverRootWidget*);

UCLASS()
class CLOVER_API UCloverUISystem : public UCloverGameSystem
{
	GENERATED_BODY()

public:
	UCloverUISystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void NotifyPlayerAdded(UCloverLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UCloverLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UCloverLocalPlayer* LocalPlayer);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "CloverUISystem", meta = (WorldContext = "WidgetContextObject"))
	static ECommonInputType GetOwningPlayerInputType(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "CloverUISystem", meta = (WorldContext = "WidgetContextObject"))
	static bool IsOwningPlayerUsingTouch(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "CloverUISystem", meta = (WorldContext = "WidgetContextObject"))
	static bool IsOwningPlayerUsingGamepad(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "CloverUISystem")
	static UCloverWidget* OpenWidget(ULocalPlayer* LocalPlayer, const ECloverWidgetLayer LayerName, const ECloverWidgetName WidgetName);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "CloverUISystem")
	static void OpenWidgetAsync(ULocalPlayer* LocalPlayer, const ECloverWidgetLayer LayerName, const ECloverWidgetName WidgetName, bool bSuspendInputUntilComplete);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "CloverUISystem")
	static void PopContentFromLayer(UCloverWidget* Widget);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "CloverUISystem")
	static ULocalPlayer* GetLocalPlayerFromController(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "CloverUISystem")
	static FName SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason);

	static FName SuspendInputForPlayer(const ULocalPlayer* LocalPlayer, FName SuspendReason);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "CloverUISystem")
	static void ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken);

	static void ResumeInputForPlayer(const ULocalPlayer* LocalPlayer, FName SuspendToken);

	FOnRootWidgetAdd OnRootWidgetAdd;
	FOnRootWidgetRemove OnRootWidgetRemove;
	FOnRootWidgetRelease OnRootWidgetRelease;

	ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const;

	UCloverWidget* InternalOpenWidget(ULocalPlayer* LocalPlayer, const ECloverWidgetLayer LayerName, const ECloverWidgetName WidgetName);
	void InternalOpenWidgetAsync(ULocalPlayer* LocalPlayer, const ECloverWidgetLayer LayerName, const ECloverWidgetName WidgetName, const bool bSuspendInputUntilComplete = true);

protected:
	void CreateLayoutWidget(UCloverLocalPlayer* LocalPlayer);
	static TSubclassOf<UCloverRootWidget> GetLayoutWidgetClass();

private:
	static int32 InputSuspensions;

	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

	void CreateRootLayoutForPlayer(UCloverLocalPlayer* LocalPlayer);
	void AddLayoutToViewport(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget);
	void RemoveLayoutFromViewport(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget);
	void OnRootLayoutAddedToViewport(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget);
	void OnRootLayoutRemoveFromViewport(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget);
	void OnRootLayoutReleased(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* RootWidget);
	void EmplaceLocalPlayerLayout(UCloverLocalPlayer* LocalPlayer, UCloverRootWidget* LayoutWidget);
	FRootViewportLayoutInfo* GetPlayerRootLayout(UCloverLocalPlayer* LocalPlayer);
	UCloverRootWidget* GetPlayerRootWidget(UCloverLocalPlayer* LocalPlayer);

	static TSubclassOf<UCloverWidget> GetWidgetClass(const ECloverWidgetName WidgetName);
	static TSoftClassPtr<UCloverWidget> GetWidgetClassSoftPtr(const ECloverWidgetName WidgetName);
};