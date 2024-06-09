// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MobaMinionSystemComponent.h"

#include "Character/MobaCharacter.h"
#include "Character/Tool/CharacterSpawnPoint.h"
#include "Common/MethodUnit.h"
#include "Game/MobaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Table/CharacterAsset.h"


UMobaMinionSystemComponent::UMobaMinionSystemComponent(): Time(4.0f), MinionCount(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMobaMinionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// 获取所有小兵生成点位置
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ACharacterSpawnPoint::StaticClass(),SpawnPoints);
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


void UMobaMinionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
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

void UMobaMinionSystemComponent::SpawnWarriorMinion()
{
	int CharacterID = 11110;
	SpawnMinion(CharacterID, RedLocations, ETeamType::ETT_Red);
	SpawnMinion(CharacterID, BlueLocations, ETeamType::ETT_Blue);
}

void UMobaMinionSystemComponent::SpawnRemoteMinion()
{
}

void UMobaMinionSystemComponent::SpawnBigMinion()
{
}

void UMobaMinionSystemComponent::SpawnSuperMinion()
{
}

void UMobaMinionSystemComponent::SpawnMinion(int32 CharacterID, TArray<FVector>& Locations, ETeamType TeamType) const
{
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		for(const auto& Location : Locations)
		{
			if(const FCharacterAsset* CharacterAsset = MobaGameState->GetCharacterAssetFromCharacterID(CharacterID))
			{
				if(CharacterAsset->CharacterClass)
				{
					if(AMobaCharacter* MobaCharacter = GetWorld()->SpawnActor<AMobaCharacter>(CharacterAsset->CharacterClass, Location, FRotator::ZeroRotator))
					{
						int64 PlayerID = FMath::RandRange(0, 1000000); 
				
						if(PlayerID!= INDEX_NONE)
						{
							MobaCharacter->RegisterCharacterOnServer(PlayerID, CharacterID, TeamType, CharacterAsset->CharacterType);
						}
					}
				}
			}
		}
	}
}
