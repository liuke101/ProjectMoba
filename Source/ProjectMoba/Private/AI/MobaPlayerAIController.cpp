// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MobaPlayerAIController.h"

#include "Common/MethodUnit.h"

AMobaPlayerAIController::AMobaPlayerAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

AMobaCharacter* AMobaPlayerAIController::FindTarget()
{
	if(bAutoFindTarget)
	{
		if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(GetCharacter()))
		{
			return MethodUnit::FindNearestTargetInRange<AMobaCharacter>(GetWorld(), 1000.0f, OwnerCharacter);
		}
	}
	
	return nullptr;
}

void AMobaPlayerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bAutoFindTarget)
	{
		if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(GetCharacter()))
		{
			FVector TargetLocation  = GetTargetLocation(GetPawn());
			if(TargetLocation != FVector::ZeroVector)
			{
				float Distance = FVector::Dist(TargetLocation, OwnerCharacter->GetActorLocation());
				if(Distance <= 80.0f)
				{
					bAutoFindTarget = true;
				}
			}
		}
	}
}

void AMobaPlayerAIController::MoveToLocation(const FVector& Destination)
{
	Super::MoveToLocation(Destination);

	bAutoFindTarget = false;
}

void AMobaPlayerAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

