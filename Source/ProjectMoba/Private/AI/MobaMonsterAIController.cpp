// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MobaMonsterAIController.h"

#include "Character/Monster/MobaMonsterCharacter.h"
#include "Common/MethodUnit.h"
#include "Kismet/GameplayStatics.h"


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
	
	CheckRange();
}

AMobaCharacter* AMobaMonsterAIController::FindTarget()
{
	if(bHoming) return nullptr;
	
	//范围内自己或友军被攻击 则选择目标
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMobaMonsterCharacter::StaticClass(), OutActors);
	
	if(AMobaMonsterCharacter* OwnerCharacter = Cast<AMobaMonsterCharacter>(GetPawn()))
	{
		for(auto& Actor : OutActors)
		{
			if(Actor != OwnerCharacter)
			{
				float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
				if(Distance<=700.f)
				{
					if(const AMobaMonsterCharacter* FriendMonster = Cast<AMobaMonsterCharacter>(Actor))
					{
						if(AMobaMonsterAIController * MonsterAIController = FriendMonster->GetController<AMobaMonsterAIController>())
						{
							if(AMobaCharacter* Target = MonsterAIController->GetTarget())
							{
								return Target;
							}
						}
					}
				}
			}
		}
	}
	return nullptr;
}

void AMobaMonsterAIController::CheckRange()
{
	float Distance = FVector::Dist(HomePoint, GetPawn()->GetActorLocation());
	// 超出范围返回出生点
	if(Distance >= 700.0f)
	{
		SetTarget(nullptr);
		MoveToLocation(HomePoint);
		bHoming = true;
	}
	
	if(Distance <= 50.0f)
	{
		bHoming = false;
	}
}

