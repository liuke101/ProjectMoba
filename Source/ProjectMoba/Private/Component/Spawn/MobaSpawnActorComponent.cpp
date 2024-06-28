// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Spawn/MobaSpawnActorComponent.h"

#include "Character/Tool/CharacterSpawnPoint.h"
#include "Common/MethodUnit.h"
#include "Game/MobaGameState.h"
#include "Table/CharacterAsset.h"


UMobaSpawnActorComponent::UMobaSpawnActorComponent()
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
}

void UMobaSpawnActorComponent::InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints)
{
}


AMobaCharacter* UMobaSpawnActorComponent::Spawn(int64 PlayerID, int32 CharacterID,  const FVector& Location, ETeamType TeamType,int32 Level) const
{
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		if(const FCharacterAsset* CharacterAsset = MobaGameState->GetCharacterAssetFromCharacterID(CharacterID))
		{
			if(CharacterAsset->CharacterClass)
			{
				if(AMobaCharacter* MobaCharacter = GetWorld()->SpawnActor<AMobaCharacter>(CharacterAsset->CharacterClass, Location, FRotator::ZeroRotator))
				{
					if(PlayerID!= INDEX_NONE)
					{
						MobaCharacter->RegisterCharacterOnServer(PlayerID, CharacterID, TeamType, CharacterAsset->CharacterType);
						MobaCharacter->Multicast_RegisterCharacter(PlayerID); //广播PlayerID
					}

					if(FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(PlayerID))
					{
						for(int32 i = 1; i < Level; i++)
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

