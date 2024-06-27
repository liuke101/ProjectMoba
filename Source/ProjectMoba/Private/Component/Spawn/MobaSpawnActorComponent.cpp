// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Spawn/MobaSpawnActorComponent.h"

#include "Character/Tool/CharacterSpawnPoint.h"
#include "Common/MethodUnit.h"
#include "Game/MobaGameState.h"
#include "Table/CharacterAsset.h"


UMobaSpawnActorComponent::UMobaSpawnActorComponent()
	: CurrentLevel(1)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMobaSpawnActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ACharacterSpawnPoint::StaticClass(),FoundActors);

	TArray<ACharacterSpawnPoint*> SpawnPoints;
	for(auto Actor : FoundActors)
	{
		if(ACharacterSpawnPoint* CharacterSpawnPoint = Cast<ACharacterSpawnPoint>(Actor))
		{
			SpawnPoints.Add(CharacterSpawnPoint);
		}
	}
	
	if(!SpawnPoints.IsEmpty())
	{
		InitSpawnPoint(SpawnPoints);
	}
}


void UMobaSpawnActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//随着时间升级
	if(CurrentLevel<=18)
	{
		if(GetWorld()->GetRealTimeSeconds() >= 60 * 3.0f) //三分钟
		{ 
			CurrentLevel = 4;
		}
		else if(GetWorld()->GetRealTimeSeconds() >= 60 * 6.0f)
		{
			CurrentLevel = 8;
		}
		else if(GetWorld()->GetRealTimeSeconds() >= 60 * 10.0f)
		{
			CurrentLevel = 12;
		}
		else if(GetWorld()->GetRealTimeSeconds() >= 60 * 15.0f)
		{
			CurrentLevel = 16;
		}
		else if(GetWorld()->GetRealTimeSeconds() >= 60 * 20.0f)
		{
			CurrentLevel = 18;
		}
	}
	
}

void UMobaSpawnActorComponent::InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints)
{
}


AMobaCharacter* UMobaSpawnActorComponent::Spawn(int32 CharacterID,  const FVector& Location, ETeamType TeamType) const
{
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
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
						MobaCharacter->Multicast_RegisterCharacter(PlayerID); //广播PlayerID
					}

					if(FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(PlayerID))
					{
						for(int32 i = 1; i < CurrentLevel; i++)
						{
							CharacterAttribute->UpdateLevel();
						}
					}

					return MobaCharacter;
				}
			}
		}
	}
	return nullptr;
}

