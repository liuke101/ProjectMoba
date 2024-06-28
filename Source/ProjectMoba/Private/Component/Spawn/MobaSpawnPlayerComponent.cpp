// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Spawn/MobaSpawnPlayerComponent.h"

#include "Character/MobaCharacter.h"
#include "Character/Hero/MobaHeroCharacter.h"
#include "Character/Tool/CharacterSpawnPoint.h"


void FPlayerCharacter::ResetDeathTime()
{
	Time = MaxTime;
}

UMobaSpawnPlayerComponent::UMobaSpawnPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMobaSpawnPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMobaSpawnPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//检测组
	CheakCroup(RedGroup,DeltaTime);
	CheakCroup(BlueGroup,DeltaTime);

	//游戏时间阶段

	//死亡倒计时
}

void UMobaSpawnPlayerComponent::InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints)
{
	auto InitPlayerSpawnPoint = [](FPlayerCharacterGroup& Group,const FVector& NewLocation)
	{
		Group.Characters.Add(FPlayerCharacter());
		FPlayerCharacter& PlayerCharacter = Group.Characters.Last();
		PlayerCharacter.SpawnPointLocation = NewLocation;
	};
	
	for(auto SpawnPoint : SpawnPoints)
	{
		if(SpawnPoint->GetCharacterType() == ECharacterType::ECT_Player)
		{
			if(SpawnPoint->GetTeamType() == ETeamType::ETT_Red)
			{
				InitPlayerSpawnPoint(RedGroup,SpawnPoint->GetActorLocation());
			}
			else if(SpawnPoint->GetTeamType() == ETeamType::ETT_Blue)
			{
				InitPlayerSpawnPoint(BlueGroup,SpawnPoint->GetActorLocation());
			}
		}
	}
}

AMobaCharacter* UMobaSpawnPlayerComponent::SpawnCharater(int64 PlayerID, int32 CharacterID, ETeamType TeamType)
{
	if(TeamType == ETeamType::ETT_Red)
	{
		return SpawnCharacterInGroup(RedGroup,PlayerID,CharacterID,TeamType);
	}
	else if(TeamType == ETeamType::ETT_Blue)
	{
		return SpawnCharacterInGroup(BlueGroup,PlayerID,CharacterID,TeamType);
	}
	
	return nullptr;
}

AMobaCharacter* UMobaSpawnPlayerComponent::SpawnCharacterInGroup(FPlayerCharacterGroup& Group, int64 PlayerID,
	int32 CharacterID, ETeamType TeamType)
{
	for(auto& Tmp : Group.Characters)
	{
		if(Tmp.Character == nullptr)
		{
			Tmp.Character = Spawn(PlayerID,CharacterID,Tmp.SpawnPointLocation, TeamType);

			//绑定委托
			if(AMobaHeroCharacter* Hero = Cast<AMobaHeroCharacter>(Tmp.Character))
			{
				Hero->OnDieDelegate.BindRaw(&Tmp, &FPlayerCharacter::ResetDeathTime);
			}
			
			return Tmp.Character;
		}
	}
	return nullptr;
}

void UMobaSpawnPlayerComponent::CheakCroup(FPlayerCharacterGroup& Group, float DeltaTime)
{
	//游戏时间阶段
	//到达10分钟时重生时间+10s
	if(GetWorld()->GetRealTimeSeconds() >= 60.0f * 10.0f)  //十分钟
	{
		ChangeTime(Group, 10.0f);
	}
	else if(GetWorld()->GetRealTimeSeconds() >= 60.0f * 20.0f)  //二十分分钟
	{
		ChangeTime(Group, 20.0f);
	}
	//else ...


	//死亡计时，重生
	for(auto& Tmp : Group.Characters)
	{
		if(Tmp.Character)
		{
			if(Tmp.Character->IsDead())
			{
				Tmp.Time -= DeltaTime;
				if(Tmp.Time <= 0.0f)
				{
					Tmp.Character->Multicast_Reborn(); 
					Tmp.ResetDeathTime();
				}
			}
		}
	}
	
}

void UMobaSpawnPlayerComponent::ChangeTime(FPlayerCharacterGroup& Group, float InNewTime)
{
	for(auto& Tmp : Group.Characters)
	{
		Tmp.MaxTime += (Tmp.MaxTime + InNewTime);
	}
}

