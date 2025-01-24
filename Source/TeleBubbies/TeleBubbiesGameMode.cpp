// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeleBubbiesGameMode.h"
#include "TeleBubbiesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATeleBubbiesGameMode::ATeleBubbiesGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
