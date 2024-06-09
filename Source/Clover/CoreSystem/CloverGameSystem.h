// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CloverGameSystem.generated.h"

/**
 * 子系统
 */
UCLASS()
class CLOVER_API UCloverGameSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

protected:
	template <class TSystem=UCloverGameSystem>
	TSystem* GetSystem() const
	{
		UGameInstance* Instance = GetGameInstance();
		return Instance ? Instance->GetSubsystem<TSystem>() : nullptr;
	}
};