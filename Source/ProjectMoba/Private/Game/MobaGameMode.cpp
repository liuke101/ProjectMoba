// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameMode.h"

#include "Game/MobaGameState.h"
#include "ProjectMoba/ProjectMobaPlayerController.h"

AMobaGameMode::AMobaGameMode()
{
	PlayerControllerClass = AProjectMobaPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_MobaPlayer"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	GameStateClass = AMobaGameState::StaticClass();
}
