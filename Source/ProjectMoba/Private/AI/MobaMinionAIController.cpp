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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMobaCharacter::StaticClass(), FoundActors);

	//收集任务目标
	FindTaskTarget(FoundActors);
	
	// 目标结构体
	FAITargetRefs AITargetRefs;

	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(GetPawn()))
	{
		/** 1. 搜索范围内各类目标 */
		for(auto& Actor : FoundActors)
		{
			if(Actor != OwnerCharacter) //排除自己
			{
				if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(Actor))
				{
					if(TargetCharacter->IsDead() || MethodUnit::IsFriendly(OwnerCharacter, TargetCharacter)) continue;
				
					float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
					
					if(Distance <= 1000.0f)
					{
						/** 目标优先级 */
						//优先级一：小兵
						if(TargetCharacter->GetCharacterType() >= ECharacterType::ECT_RemoteMinion && TargetCharacter->GetCharacterType() <= ECharacterType::ECT_SuperMinion)
						{
							AITargetRefs.MinionTargets.Add(FAITargetRef{TargetCharacter, Distance});
						}
						//优先级二：野怪
						else if(TargetCharacter->GetCharacterType() == ECharacterType::ECT_WildMonster || TargetCharacter->GetCharacterType() == ECharacterType::ECT_BossMonster)
						{
							AITargetRefs.MonsterTargets.Add(FAITargetRef{TargetCharacter, Distance});
						}
						//优先级三：炮塔
						else if(TargetCharacter->GetCharacterType() >= ECharacterType::ECT_1st_Tower && TargetCharacter->GetCharacterType() <= ECharacterType::ECT_Base_Tower)
						{
							AITargetRefs.TowerTargets.Add(FAITargetRef{TargetCharacter, Distance});
						}
						//优先级四：玩家
						else if(TargetCharacter->GetCharacterType() == ECharacterType::ECT_Player)
						{
							AITargetRefs.PlayerTargets.Add(FAITargetRef{TargetCharacter, Distance});
						}
					}
				}
			}
		}

		/** 2. 查询(友军)目标数 */
		for(auto& Actor : FoundActors)
		{
			if(Actor != OwnerCharacter) //排除自己
			{
				if(AMobaCharacter* FriendlyCharacter = Cast<AMobaCharacter>(Actor))
				{
					if(FriendlyCharacter->IsDead()) continue;

					//如果是友军
					if(MethodUnit::IsFriendly(OwnerCharacter, FriendlyCharacter))
					{
						if(AMobaAIController* MobaAIController = FriendlyCharacter->GetController<AMobaAIController>())
						{
							//将友军的目标传入
							AITargetRefs.InitRef(MobaAIController->GetTarget());
						}
					}
				}
			}
		}
	}

	/** 3. 获取最符合条件的目标 */
	AMobaCharacter* TargetCharacter = AITargetRefs.GetTarget();
	
	//如果没有找到目标，就返回任务目标
	if(!TargetCharacter)
	{
		return GetTaskTarget();
	}
	
	return TargetCharacter;
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

bool AMobaMinionAIController::CheckRange() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMobaCharacter::StaticClass(), FoundActors);

	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(GetPawn()))
	{
		for(auto& Actor : FoundActors)
		{
			if(Actor != OwnerCharacter) //排除自己
			{
				if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(Actor))
				{
					if(TargetCharacter->IsDead() || MethodUnit::IsFriendly(OwnerCharacter, TargetCharacter)) continue;
				
					float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
					
					if(Distance <= 1000.0f)
					{
						return true;
					}
				}
			}
		}
	}
	
	return false;
}

AMobaCharacter* FAITargetRefs::GetTarget()
{
	AMobaCharacter* ClosestTarget = nullptr;
	//1. 优先选择 被引用的次数最低的目标
	const int32 MinRefCount = GetMinRefCount();
	
	if(MinRefCount != INT_MAX)
	{
		//2. 获取 满足基准线内距离自己最近的敌人
		// lambda
		auto GetClosestTargetWithMinRefCount = [&](TArray<FAITargetRef>& Targets)
		{
			TArray<FAITargetRef*> OutTargetsWithMinRefCount;
			GetTargetsWithMinRefCount(MinRefCount, Targets, OutTargetsWithMinRefCount);
			ClosestTarget = GetClosestTarget(OutTargetsWithMinRefCount);
			
			return ClosestTarget;
		};
		
		//按优先级选择目标
		if(!GetClosestTargetWithMinRefCount(MinionTargets))
		{
			if(!GetClosestTargetWithMinRefCount(MonsterTargets))
			{
				if(!GetClosestTargetWithMinRefCount(TowerTargets))
				{
					if(!GetClosestTargetWithMinRefCount(PlayerTargets))
					{
						return nullptr;
					}
				}
			}
		}
	}
	
	return ClosestTarget;
}

void FAITargetRefs::InitRef(const AMobaCharacter* TargetCharacter)
{
	if(InitTargetRef(MinionTargets, TargetCharacter))
	{
		
	}
	else if(InitTargetRef(MonsterTargets, TargetCharacter))
	{
		
	}
	else if(InitTargetRef(TowerTargets, TargetCharacter))
	{
		
	}
	else if(InitTargetRef(PlayerTargets, TargetCharacter))
	{
		
	}
}

int32 FAITargetRefs::GetMinRefCount() const
{
	//获取四个数组中最小的引用次数
	int MinRefCount = INT_MAX;

	//lambda
	auto GetMinRefCountFromAITargetRefs = [&](const TArray<FAITargetRef>& TargetRefs)
	{
		for(auto& TargetRef : TargetRefs)
		{
			if(TargetRef.RefCount < MinRefCount)
			{
				MinRefCount = TargetRef.RefCount;
			}
		}
	};

	GetMinRefCountFromAITargetRefs(MinionTargets);
	GetMinRefCountFromAITargetRefs(MonsterTargets);
	GetMinRefCountFromAITargetRefs(TowerTargets);
	GetMinRefCountFromAITargetRefs(PlayerTargets);
	
	return MinRefCount;
}

void FAITargetRefs::GetTargetsWithMinRefCount(int32 MinRefCount, TArray<FAITargetRef>& InTargetRefs,
	TArray<FAITargetRef*>& OutTargetsWithMinRefCount)
{
	for(auto& TargetRef : InTargetRefs)
	{
		if(TargetRef.RefCount == MinRefCount)
		{
			OutTargetsWithMinRefCount.Add(&TargetRef);
		}
	}
}

AMobaCharacter* FAITargetRefs::GetClosestTarget(const TArray<FAITargetRef*>& MinRefCountTargets)
{
	AMobaCharacter* ClosestCharacter = nullptr;
	float MinDistance = INT_MAX;
	
	for(auto& TargetRef : MinRefCountTargets)
	{
		if(TargetRef->Distance < MinDistance)
		{
			MinDistance = TargetRef->Distance;
			ClosestCharacter = TargetRef->TargetCharacter;
		}
	}

	return ClosestCharacter;
}


bool FAITargetRefs::InitTargetRef(TArray<FAITargetRef>& TargetRefs, const AMobaCharacter* TargetCharacter)
{
	for(auto TargetRef : TargetRefs)
	{
		if(TargetRef.TargetCharacter == TargetCharacter)
		{
			TargetRef.RefCount++;
			return true;
		}
	}

	return false;
}

