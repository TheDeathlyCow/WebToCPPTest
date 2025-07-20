// Copyright Epic Games, Inc. All Rights Reserved.

#include "WebToCPPTestGameMode.h"
#include "WebToCPPTestHUD.h"
#include "WebToCPPTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWebToCPPTestGameMode::AWebToCPPTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AWebToCPPTestHUD::StaticClass();
}
