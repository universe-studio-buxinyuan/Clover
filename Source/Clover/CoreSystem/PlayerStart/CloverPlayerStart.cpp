// Copyright Clover Games, Inc. All Rights Reserved.


#include "CloverPlayerStart.h"


// Sets default values
ACloverPlayerStart::ACloverPlayerStart()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACloverPlayerStart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACloverPlayerStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

