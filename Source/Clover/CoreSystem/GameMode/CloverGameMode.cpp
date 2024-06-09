// Copyright Clover Games, Inc. All Rights Reserved.

#include "CloverGameMode.h"
#include "UObject/ConstructorHelpers.h"

ACloverGameMode::ACloverGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
