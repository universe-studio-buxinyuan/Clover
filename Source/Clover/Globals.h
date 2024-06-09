// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Globals.generated.h"

class UCloverGameSystem;

DEFINE_LOG_CATEGORY_STATIC(LogClover, Log, All);

UCLASS()
class CLOVER_API UGlobals : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <class TSystem=UCloverGameSystem>
	static TSystem* GetSystem(UObject* WorldContextObject)
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		return GameInstance ? GameInstance->GetSubsystem<TSystem>() : nullptr;
	}
};