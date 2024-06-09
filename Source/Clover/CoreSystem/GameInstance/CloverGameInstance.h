// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "CloverGameInstance.generated.h"

enum class ECommonUserAvailability : uint8;
enum class ECommonUserPrivilege : uint8;

class FText;
class UCommonUserInfo;
class UCommonSession_SearchResult;
struct FOnlineResultInformation;
class ULocalPlayer;
class USocialManager;
class UObject;
struct FFrame;
struct FGameplayTag;

UCLASS(Config = Game)
class CLOVER_API UCloverGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	explicit UCloverGameInstance(const FObjectInitializer& ObjectInitializer);
};