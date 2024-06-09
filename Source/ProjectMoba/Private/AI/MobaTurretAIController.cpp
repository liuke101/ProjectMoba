﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MobaTurretAIController.h"

#include "Character/Turret/MobaTurretCharacter.h"
#include "Common/MethodUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Table/CharacterAttribute.h"


AMobaTurretAIController::AMobaTurretAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMobaTurretAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetTarget())
	{
		if(AMobaTurretCharacter* OwnerCharacter = GetPawn<AMobaTurretCharacter>())
		{
			// 炮塔旋转朝向目标
			FRotator R = (GetTarget()->GetActorLocation() - GetPawn()->GetActorLocation()).ToOrientationRotator();
			OwnerCharacter->TurretRotator = FVector2D(R.Pitch, R.Yaw);
			if(GetPawn()->GetActorRotation() != FRotator::ZeroRotator)
			{
				// 修正炮塔旋转
				FVector2D NewRotation = FVector2D(GetPawn()->GetActorRotation().Pitch, GetPawn()->GetActorRotation().Yaw);
				OwnerCharacter->TurretRotator -= NewRotation;
			}
		}
	}
}

void AMobaTurretAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMobaTurretAIController::InitMobaAIController()
{
	Super::InitMobaAIController();
}

void AMobaTurretAIController::SetTarget(AMobaCharacter* InTarget)
{
	Super::SetTarget(InTarget);

	if(InTarget == nullptr)
	{
		bTargetInAttackRange = false;
	}
	else
	{
		bTargetInAttackRange = true;
	}
}

AMobaCharacter* AMobaTurretAIController::FindTarget()
{
	//范围检测
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMobaCharacter::StaticClass(), FoundActors);

	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(GetPawn()))
	{
		if(!bTargetInAttackRange)
		{
			float OwnerDistance = 999999.0f;
			AMobaCharacter* TargetCharacter= nullptr;
			
			for(auto& Actor : FoundActors)
			{
				if(Actor != OwnerCharacter)
				{
					if(AMobaCharacter* InTargetCharacter = Cast<AMobaCharacter>(Actor))
					{
						//友军检测
						if(InTargetCharacter->IsDead() || MethodUnit::IsFriendly(OwnerCharacter, InTargetCharacter)) continue;
						
						float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
						float AttackRange = OwnerCharacter->GetCharacterAttribute()->AttackRange;
						if(Distance <= AttackRange)
						{
							if(Distance < OwnerDistance)
							{
								OwnerDistance = Distance;
								TargetCharacter = InTargetCharacter;
								bTargetInAttackRange = true;
							}
						}
					}
				}
				
			}
			
			return TargetCharacter;
		}
		
		
		// 目标结构体
		struct FAITarget
		{
			AMobaCharacter* Minion = nullptr;
			AMobaCharacter* Player = nullptr;
			AMobaCharacter* Monster = nullptr;
			
			float MinionMinDistance = INT_MAX;
			float PlayerMinDistance = INT_MAX;
			float MonsterMinDistance = INT_MAX;
		};
		
		// 搜索攻击范围内各类别最近目标
		FAITarget AITarget;
		for(auto& Actor : FoundActors)
		{
			if(Actor != OwnerCharacter)
			{
				if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(Actor))
				{
					//友军检测
					if(TargetCharacter->IsDead() || MethodUnit::IsFriendly(OwnerCharacter, TargetCharacter)) continue;
					
					float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
					float AttackRange = OwnerCharacter->GetCharacterAttribute()->AttackRange;
					
					if(Distance <= AttackRange)
					{
						/** 目标优先级 */
						//优先级一：小兵
						if(TargetCharacter->GetCharacterType() >= ECharacterType::ECT_RemoteMinion && TargetCharacter->GetCharacterType() <= ECharacterType::ECT_SuperMinion)
						{
							if(Distance < AITarget.MinionMinDistance)
							{
								AITarget.MinionMinDistance = Distance;
								AITarget.Minion = TargetCharacter;
							}
						}
						//优先级二：玩家
						else if(TargetCharacter->GetCharacterType() == ECharacterType::ECT_Player)
						{
							if(Distance < AITarget.PlayerMinDistance)
							{
								AITarget.PlayerMinDistance = Distance;
								AITarget.Player = TargetCharacter;
							}
						}
						//优先级三：野怪
						else if(TargetCharacter->GetCharacterType() == ECharacterType::ECT_WildMonster || TargetCharacter->GetCharacterType() == ECharacterType::ECT_BossMonster)
						{
							if(Distance < AITarget.MonsterMinDistance)
							{
								AITarget.MonsterMinDistance = Distance;
								AITarget.Monster = TargetCharacter;
							}
						}
					}
				}
			}
		}

		// 按照优先级返回最近目标
		if(AITarget.Monster)
		{
			return AITarget.Monster;
		}
		if(AITarget.Minion)
		{
			return AITarget.Minion;
		}
		if(AITarget.Player)
		{
			return AITarget.Player;
		}
		
	}
	return nullptr;
}

