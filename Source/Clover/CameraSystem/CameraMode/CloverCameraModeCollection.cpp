// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverCameraModeCollection.h"

#include "CloverCameraMode.h"
#include "Engine/Canvas.h"

UCloverCameraModeCollection::UCloverCameraModeCollection() {}

void UCloverCameraModeCollection::SwitchMode(const TSubclassOf<UCloverCameraMode>& ModeClass)
{
	if (!ModeClass)
	{
		return;
	}

	UCloverCameraMode* CameraMode = GetModeInstance(ModeClass);
	check(CameraMode);

	int32 StackSize = ModeStack.Num();
	if ((StackSize > 0) && ModeStack[0] == CameraMode)
	{
		//切换模式已在栈顶, 不切换
		return;
	}

	//检查当前模式是否已经在栈里
	//计算当前模式进度
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		TObjectPtr<UCloverCameraMode> ModeInstance = ModeStack[StackIndex];
		if (ModeInstance == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->GetBlendWeight();
			break;
		}
		ExistingStackContribution *= 1.0f - ModeInstance->GetBlendWeight();
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		ModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = CameraMode->GetBlendTime() > 0.0f && StackSize > 0;
	const float BlendWeight = bShouldBlend ? ExistingStackContribution : 1.0f;
	CameraMode->SetBlendWeightAndAlpha(BlendWeight);
	ModeStack.Insert(CameraMode, 0);
	ModeStack.Last()->SetBlendWeightAndAlpha(1.0f);
}

FOnCameraModeChange& UCloverCameraModeCollection::OnModeChange()
{
	return OnCameraModeChange;
}

FCloverCameraModeData& UCloverCameraModeCollection::Evaluate(AActor* Target, const float DeltaTime)
{
	const int32 StackSize = ModeStack.Num();
	if (StackSize <= 0)
	{
		return EmptyModeData;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UCloverCameraMode* CameraMode = ModeStack[StackIndex];
		check(CameraMode);

		CameraMode->Evaluate(Target, DeltaTime);
		if (CameraMode->GetBlendWeight() >= 1.0f)
		{
			RemoveIndex = StackIndex + 1;
			RemoveCount = StackSize - RemoveIndex;
			break;
		}
	}

	if (RemoveCount > 0)
	{
		ModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}

	const int32 CurSize = ModeStack.Num();
	if (CurSize <= 0)
	{
		return EmptyModeData;
	}

	//从栈底状态开始过渡
	const TObjectPtr<UCloverCameraMode> CameraMode = ModeStack[StackSize - 1];
	check(CameraMode);
	FCloverCameraModeData& ModeData = CameraMode->GetModeData();
	for (int32 StackIndex = StackSize - 2; StackIndex >= 0; --StackIndex)
	{
		TObjectPtr<UCloverCameraMode> Mode = ModeStack[StackIndex];
		check(Mode);
		ModeData.Blend(Mode->GetModeData(), Mode->GetBlendWeight());
	}
	return ModeData;
}

void UCloverCameraModeCollection::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetDrawColor(FColor::Green);
	DisplayDebugManager.DrawString(FString(TEXT("   --- Camera Modes (Begin) ---")));

	for (const UCloverCameraMode* CameraMode : ModeStack)
	{
		check(CameraMode);
		CameraMode->DrawDebug(Canvas);
	}

	DisplayDebugManager.SetDrawColor(FColor::Green);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   --- Camera Modes (End) ---")));
}

UCloverCameraMode* UCloverCameraModeCollection::GetModeInstance(const TSubclassOf<UCloverCameraMode>& ModeClass)
{
	check(ModeClass);
	for (UCloverCameraMode* CameraMode : ModeInstances)
	{
		if (CameraMode != nullptr && CameraMode->GetClass() == ModeClass)
		{
			return CameraMode;
		}
	}
	UCloverCameraMode* NewMode = NewObject<UCloverCameraMode>(GetOuter(), ModeClass, NAME_None, RF_NoFlags);
	check(NewMode)
	ModeInstances.Add(NewMode);
	return NewMode;
}