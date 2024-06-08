﻿#include "AI/MobaMinionAIController.h"

#include "Character/Hero/MobaHeroCharacter.h"
#include "Common/MethodUnit.h"
#include "Kismet/GameplayStatics.h"


AMobaMinionAIController::AMobaMinionAIController()
{
}

void AMobaMinionAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMobaMinionAIController::InitMobaAIController()
{
	Super::InitMobaAIController();
}

AMobaCharacter* AMobaMinionAIController::FindTarget()
{
	//范围检测
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMobaCharacter::StaticClass(), FoundActors);

	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(GetPawn()))
	{
		// 目标结构体
		struct FAITarget
		{
			AMobaCharacter* Minion = nullptr;
			AMobaCharacter* Tower = nullptr;
			AMobaCharacter* Player = nullptr;
			AMobaCharacter* Monster = nullptr;
			
			float MinionMinDistance = INT_MAX;
			float TowerMinDistance = INT_MAX;
			float PlayerMinDistance = INT_MAX;
			float MonsterMinDistance = INT_MAX;
		};
		
		// 搜索2000范围内各类别最近目标
		FAITarget AITarget;
		for(auto& Actor : FoundActors)
		{
			if(Actor != OwnerCharacter) //排除自己
			{
				if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(Actor))
				{
					//友军检测
					if(MethodUnit::IsFriendly(OwnerCharacter, TargetCharacter)) continue;
				
					float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
					if(Distance <= 2000.0f)
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
						//优先级二：炮塔
						else if(TargetCharacter->GetCharacterType() >= ECharacterType::ECT_1st_Tower && TargetCharacter->GetCharacterType() <= ECharacterType::ECT_Base_Tower)
						{
							if(Distance < AITarget.TowerMinDistance)
							{
								AITarget.TowerMinDistance = Distance;
								AITarget.Tower = TargetCharacter;
							}
						}
						//优先级三：玩家
						else if(TargetCharacter->GetCharacterType() == ECharacterType::ECT_Player)
						{
							if(Distance < AITarget.PlayerMinDistance)
							{
								AITarget.PlayerMinDistance = Distance;
								AITarget.Player = TargetCharacter;
							}
						}
						//优先级四：野怪
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
		if(AITarget.Minion)
		{
			return AITarget.Minion;
		}
		if(AITarget.Tower)
		{
			return AITarget.Tower;
		}
		if(AITarget.Player)
		{
			return AITarget.Player;
		}
		if(AITarget.Monster)
		{
			return AITarget.Monster;
		}
	}
	return nullptr;
}

