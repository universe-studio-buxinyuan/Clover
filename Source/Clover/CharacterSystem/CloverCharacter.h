// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CloverCharacter.generated.h"

class UCloverCameraComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogCloverCharacter, Log, All);

UCLASS(config=Clover)
class ACloverCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit ACloverCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	FORCEINLINE TObjectPtr<UCloverCameraComponent> GetFollowCamera() const;

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clover | Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCloverCameraComponent> CameraComponent;

	void SyncCapsule();
	void SyncSkeletalMesh();
	void SyncMovement();
};