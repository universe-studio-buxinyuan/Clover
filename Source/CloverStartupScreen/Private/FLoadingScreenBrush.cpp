// Copyright Clover Games, Inc. All Rights Reserved.


#include "FLoadingScreenBrush.h"

FLoadingScreenBrush::FLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize): FSlateDynamicImageBrush(InTextureName, InImageSize)
{
	SetResourceObject(LoadObject<UObject>(nullptr, *InTextureName.ToString()));
}

void FLoadingScreenBrush::AddReferencedObjects(FReferenceCollector& Collector)
{
	if (TObjectPtr<UObject> CachedResourceObject = GetResourceObject())
	{
		Collector.AddReferencedObject(CachedResourceObject);
	}
}

FString FLoadingScreenBrush::GetReferencerName() const
{
	return TEXT("SCloverPreLoadingScreenWidget");
}