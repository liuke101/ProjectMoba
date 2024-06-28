// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/Spawn/MobaSpawnMinionComponent.h"
#include "Character/Tool/CharacterSpawnPoint.h"

UMobaSpawnMinionComponent::UMobaSpawnMinionComponent(): Time(4.0f), MinionCount(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMobaSpawnMinionComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UMobaSpawnMinionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Time -= DeltaTime;
	if (Time <= 0)
	{
		Time = 1.0f;

		switch (MinionCount)
		{
		case 0:
		case 1:
		case 2:
			SpawnWarriorMinion();
			break;
		default:
			break;
			
		}
		
		MinionCount++;

		if(MinionCount > 2)
		{
			MinionCount = 0;
			Time = 20.0f;
		}
	}
}

void UMobaSpawnMinionComponent::InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints)
{
	for(auto& SpawnPoint : SpawnPoints)
	{
		if(ACharacterSpawnPoint* CharacterSpawnPoint = Cast<ACharacterSpawnPoint>(SpawnPoint))
		{
			if(CharacterSpawnPoint->GetCharacterType() >= ECharacterType::ECT_WarriorMinion &&
				CharacterSpawnPoint->GetCharacterType() <= ECharacterType::ECT_SuperMinion)
			{
				if (CharacterSpawnPoint->GetTeamType() == ETeamType::ETT_Red)
				{
					RedLocations.Add(CharacterSpawnPoint->GetActorLocation());
				}
				else if (CharacterSpawnPoint->GetTeamType() == ETeamType::ETT_Blue)
				{
					BlueLocations.Add(CharacterSpawnPoint->GetActorLocation());
				}
			}
		}
	}
}

void UMobaSpawnMinionComponent::SpawnWarriorMinion()
{
	
	int CharacterID = 11110;
	for(auto& RedLocation : RedLocations)
	{
		int64 PlayerID = FMath::RandRange(0,999999);
		Spawn(PlayerID, CharacterID, RedLocation, ETeamType::ETT_Red, GetCurrentLevel());
	}

	for(auto& BlueLocation: BlueLocations)
	{
		int64 PlayerID = FMath::RandRange(0,999999);
		Spawn(PlayerID, CharacterID, BlueLocation, ETeamType::ETT_Blue, GetCurrentLevel());
	}
}

void UMobaSpawnMinionComponent::SpawnRemoteMinion()
{
}

void UMobaSpawnMinionComponent::SpawnBigMinion()
{
}

void UMobaSpawnMinionComponent::SpawnSuperMinion()
{
}
