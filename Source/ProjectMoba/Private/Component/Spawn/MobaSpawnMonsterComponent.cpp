// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/Spawn/MobaSpawnMonsterComponent.h"

#include "ThreadManage.h"
#include "Character/Monster/MobaMonsterCharacter.h"
#include "Character/Tool/CharacterSpawnPoint.h"


FMonster::FMonster()
	: SpawnPoint(FVector::ZeroVector)
{
}

FMonsterGroup::FMonsterGroup()
	: CharacterType(ECharacterType::ECT_WildMonster)
	, bDieGroup(false)
	, SpawnTime(60.0f)
	, MaxSpawnTime(60.0f)
{
}

UMobaSpawnMonsterComponent::UMobaSpawnMonsterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMobaSpawnMonsterComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMobaSpawnMonsterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//生命周期管理
	for(auto& Group : MonsterGroups)
	{
		if(!Group.bDieGroup)
		{
			bool bDieGroup = true;

			//是否有活着的
			for(auto& Monster: Group.Monsters)
			{
				if(Monster.MonsterPtr.IsValid())
				{
					if(!Monster.MonsterPtr->IsDead())
					{
						bDieGroup = false;
						break;
					}
				}
			}

			//如果全死了
			if(bDieGroup)
			{
				Group.bDieGroup = true;
				Group.SpawnTime = Group.MaxSpawnTime;
			}
		}
	}

	//组内全死了才生成新的
	for(auto& Group : MonsterGroups)
	{
		if(Group.bDieGroup)
		{
			//倒计时
			Group.SpawnTime -= DeltaTime;
			if(Group.SpawnTime <= 0.0f)
			{
				SpawnMonster(Group);
				Group.bDieGroup = false;
			}
		}
	}
	
}

void UMobaSpawnMonsterComponent::InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints)
{
	for(auto& SpawnPoint : SpawnPoints)
	{
		if(ACharacterSpawnPoint* CharacterSpawnPoint = Cast<ACharacterSpawnPoint>(SpawnPoint))
		{
			if(CharacterSpawnPoint->GetCharacterType() >= ECharacterType::ECT_WildMonster &&
				CharacterSpawnPoint->GetCharacterType() <= ECharacterType::ECT_BossMonster)
			{
				AllocationGroup(SpawnPoint);
			}
		}
	}

	//60s后生成野怪
	// GThread::GetCoroutines().BindLambda(60.0f, [&]()
	// {
	// 	for(auto& Tmp : MonsterGroups)
	// 	{
	// 		SpawnMonster(Tmp);
	// 	}
	// });
	
}

void UMobaSpawnMonsterComponent::AllocationGroup(ACharacterSpawnPoint* SpawnPoint)
{
	auto AddMonster = [&](FMonsterGroup& NewGroup)
	{
		NewGroup.Monsters.Add(FMonster()); //将monster添加进当前组
		FMonster& NewMonster = NewGroup.Monsters.Last();
		NewMonster.SpawnPoint = SpawnPoint->GetActorLocation();
	};

	bool bInGroup = false;
	for(auto& MonsterGroup : MonsterGroups)
	{
		bool bIsGroup = false; //是否是一个组

		for(auto& Monster : MonsterGroup.Monsters)
		{
			float Distance = FVector::Dist(Monster.SpawnPoint, SpawnPoint->GetActorLocation());
			
			if(Distance<=GroupRange)
			{
				bIsGroup = true;
				break;
			}
		}

		if(bIsGroup)
		{
			AddMonster(MonsterGroup);
			bInGroup = true;
		}
	}
	
	if(!bInGroup)
	{
		MonsterGroups.Add(FMonsterGroup()); //添加新组
		FMonsterGroup& NewGroup = MonsterGroups.Last();

		//时间初始化
		NewGroup.SpawnTime = SpawnPoint->GetSpawnTime();
		NewGroup.MaxSpawnTime = SpawnPoint->GetSpawnTime();
			
		AddMonster(NewGroup);
	}
}

void UMobaSpawnMonsterComponent::SpawnMonster(FMonsterGroup& MonsterGroup)
{
	for(auto& Tmp : MonsterGroup.Monsters)
	{
		switch (MonsterGroup.CharacterType)
		{
		case ECharacterType::ECT_WildMonster:
			Tmp.MonsterPtr = Spawn(33330, Tmp.SpawnPoint, ETeamType::ETT_Neutral);
			break;
		case ECharacterType::ECT_BossMonster:
			Tmp.MonsterPtr = Spawn(33330, Tmp.SpawnPoint, ETeamType::ETT_Neutral);
			break;
		default:
			break;
		}
	}
}

