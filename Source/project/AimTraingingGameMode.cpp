// Fill out your copyright notice in the Description page of Project Settings.


#include "AimTraingingGameMode.h"
#include "TrainingPlayerController.h"

AAimTraingingGameMode::AAimTraingingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_TrainingCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = ATrainingPlayerController::StaticClass();
	}


}
