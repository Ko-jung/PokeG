// Copyright Epic Games, Inc. All Rights Reserved.

#include "PokeGGameMode.h"
#include "PokeGCharacter.h"
#include "UObject/ConstructorHelpers.h"

APokeGGameMode::APokeGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
