// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MobaMonsterAIController.h"


AMobaMonsterAIController::AMobaMonsterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMobaMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMobaMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AMobaCharacter* AMobaMonsterAIController::FindTarget()
{
	return nullptr;
}

