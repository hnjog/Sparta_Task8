// Copyright Epic Games, Inc. All Rights Reserved.

#include "Task8GameMode.h"
#include "Task8PlayerController.h"
#include "Task8Character.h"
#include "UObject/ConstructorHelpers.h"

ATask8GameMode::ATask8GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATask8PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}