// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMORPGGameMode.h"
#include "MMORPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMMORPGGameMode::AMMORPGGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	 static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/MP_ThirdPerson"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		
	}
}
