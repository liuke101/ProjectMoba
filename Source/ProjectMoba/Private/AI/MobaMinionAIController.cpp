#include "AI/MobaMinionAIController.h"

#include "Character/Hero/MobaHero.h"
#include "Kismet/GameplayStatics.h"


AMobaMinionAIController::AMobaMinionAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMobaMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMobaMinionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AMobaCharacter* AMobaMinionAIController::FindTarget()
{
	//范围检测
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMobaCharacter::StaticClass(), FoundActors);

	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(GetPawn()))
	{
		// 保存距离2000以内的Actor
		TArray<AActor*> TargetActors;
		for(auto& Actor : FoundActors)
		{
			if(Actor != OwnerCharacter)
			{
				float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
				if(Distance <= 2000.0f)
				{
					TargetActors.Add(Actor);
				}
			}
		}

		for(auto& TargetActor : TargetActors)
		{
			if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(TargetActor))
			{
				/** 目标优先级 */
				//优先级一：小兵
				if(TargetCharacter->GetCharacterType() >= ECharacterType::ECT_RemoteMinion && TargetCharacter->GetCharacterType() <= ECharacterType::ECT_SuperMinion)
				{
					return TargetCharacter;
				}
				//优先级二：炮塔
				else if(TargetCharacter->GetCharacterType() >= ECharacterType::ECT_1st_Tower && TargetCharacter->GetCharacterType() <= ECharacterType::ECT_Base_Tower)
				{
					return TargetCharacter;
				}
				//优先级三：玩家
				else if(TargetCharacter->GetCharacterType() == ECharacterType::ECT_Player)
				{
					return TargetCharacter;
				}
				//优先级四：野怪
				else if(TargetCharacter->GetCharacterType() == ECharacterType::ECT_WildMonster)
				{
					return TargetCharacter;
				}
				//优先级五：路径
				else 
				{
					return nullptr;
				}
			}
		}
	}
	return nullptr;
}

