#include "AI/MobaMinionAIController.h"

#include "Character/Hero/MobaHeroCharacter.h"
#include "Common/MethodUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Table/CharacterAttribute.h"


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

	//收集任务目标
	FindTaskTarget(FoundActors);
	
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
		
		// 搜索攻击范围内各类别最近目标
		FAITarget AITarget;
		
		for(auto& Actor : FoundActors)
		{
			if(Actor != OwnerCharacter) //排除自己
			{
				if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(Actor))
				{
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
		AMobaCharacter* TargetCharacter = nullptr;
		if(AITarget.Minion)
		{
			TargetCharacter = AITarget.Minion;
		}
		if(AITarget.Tower)
		{
			TargetCharacter = AITarget.Tower;
		}
		if(AITarget.Player)
		{
			TargetCharacter = AITarget.Player;
		}
		if(AITarget.Monster)
		{
			TargetCharacter = AITarget.Monster;
		}

		if(TargetCharacter)
		{
			return TargetCharacter;
		}
		else
		{
			return GetTaskTarget();
		}
	}
	return nullptr;
}

AMobaCharacter* AMobaMinionAIController::GetTaskTarget()
{
	return TaskTarget;
}

void AMobaMinionAIController::FindTaskTarget(const TArray<AActor*>& OutActors)
{

	if(!TaskTarget || TaskTarget->IsDead())
	{
		if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(GetPawn()))
		{
			float MinDistance = INT_MAX;
			for(auto& Actor : OutActors)
			{
				if(Actor != OwnerCharacter) //排除自己
				{
					if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(Actor))
					{
						if(TargetCharacter->IsDead() || MethodUnit::IsFriendly(OwnerCharacter, TargetCharacter)) continue;

						//寻找最近炮塔
						if(TargetCharacter->GetCharacterType() >= ECharacterType::ECT_1st_Tower && TargetCharacter->GetCharacterType() <= ECharacterType::ECT_Base_Tower)
						{
							float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
							if(Distance < MinDistance)
							{
								MinDistance = Distance;
								TaskTarget = TargetCharacter;
							}
						}
					}
				}
			}
		}
	}
}

