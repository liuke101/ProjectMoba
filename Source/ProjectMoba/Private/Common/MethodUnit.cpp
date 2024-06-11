// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MethodUnit.h"

#include "Character/MobaCharacter.h"
#include "Component/PlayerDataComponent.h"
#include "Game/MobaGameState.h"
#include "Kismet/GameplayStatics.h"

AMobaGameState* MethodUnit::GetMobaGameState(const UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetGameState<AMobaGameState>();
	}
	return nullptr;
}

AMobaPlayerState* MethodUnit::GetMobaPlayerStateFromPlayerID(UWorld* InWorld, const int64 PlayerID)
{
	if(InWorld)
	{
		AMobaPlayerState* MobaPlayerState = nullptr;
		ServerCallAllPlayerState<AMobaPlayerState>(InWorld, [&](AMobaPlayerState* PlayerState)
		{
			if(PlayerState->GetPlayerID() == PlayerID)
			{
				MobaPlayerState = PlayerState;
				return EServerCallType::ECT_ProgressComplete;
			}
			return EServerCallType::ECT_InProgress;
		});

		return MobaPlayerState;
	}
	return nullptr;
	
}

AMobaPlayerState* MethodUnit::GetMobaPlayerState(UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetFirstPlayerController()->GetPlayerState<AMobaPlayerState>();
	}
	return nullptr;
}

const FCharacterAsset* MethodUnit::GetCharacterAssetFromCharacterID(const UWorld* InWorld, const int32 CharacterID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAssetFromCharacterID(CharacterID);
	}
	return nullptr;
}

const FCharacterAsset* MethodUnit::GetCharacterAssetFromPlayerID(const UWorld* InWorld, int64 PlayerID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAssetFromPlayerID(PlayerID);
	}
	return nullptr;
}

const FCharacterAttribute* MethodUnit::GetCharacterAttributeFromCharacterID(const UWorld* InWorld, const int32 CharacterID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAttributeFromCharacterID(CharacterID);
	}
	return nullptr;
}


FCharacterAttribute* MethodUnit::GetCharacterAttributeFromPlayerID(const UWorld* InWorld, int64 PlayerID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAttributeFromPlayerID(PlayerID);
	}
	return nullptr;
}


const FSlotAttribute* MethodUnit::GetSlotAttributeFromAnimMontage(AMobaPlayerState* PlayerState,
                                                            const UAnimMontage* AnimMontage)
{
	int32 SkillDataID = INDEX_NONE;
	
	if(PlayerState)
	{
		//根据AnimMontage获取技能的DataID
		if(AMobaGameState* MobaGameState = GetMobaGameState(PlayerState->GetWorld()))
		{
			SkillDataID = MobaGameState->GetSkillDataIDFromAnimMontage(AnimMontage);
		}

		//根据DataID获取SlotAttribute
		if(SkillDataID!=INDEX_NONE)
		{
			return PlayerState->GetSlotAttributeFromDataID(SkillDataID);
		}
	}

	return nullptr;
}

AMobaPawn* MethodUnit::GetMobaPawnFromPlayerID(UWorld* InWorld, int64 PlayerID)
{
	AMobaPawn* MobaPawn = nullptr;
	ServerCallAllPlayer<AMobaPawn>(InWorld, [&](AMobaPawn* InMobaPawn)
	{
		if(InMobaPawn->GetPlayerID() == PlayerID)
		{
			MobaPawn = InMobaPawn;
			return EServerCallType::ECT_ProgressComplete;
		}
		return EServerCallType::ECT_InProgress;
	});

	return MobaPawn;
}

AMobaPawn* MethodUnit::GetMobaPawn(UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetFirstPlayerController()->GetPawn<AMobaPawn>();
	}
	return nullptr;
}


AMobaCharacter* MethodUnit::GetMobaCharacterFromPlayerID(UWorld* InWorld, int64 PlayerID)
{
	TArray<AActor*> FindActors{};
	UGameplayStatics::GetAllActorsOfClass(InWorld, AMobaCharacter::StaticClass(), FindActors);
	for(AActor* Actor : FindActors)
	{
		if(AMobaCharacter* MobaCharacter = Cast<AMobaCharacter>(Actor))
		{
			if(MobaCharacter->GetPlayerID() == PlayerID)
			{
				return MobaCharacter;
			}
		}
	}
	return nullptr;
}

bool MethodUnit::IsPlayer(UWorld* InWorld, int64 InPlayerID)
{
	ServerCallAllPlayerDataComponent<UPlayerDataComponent>(InWorld, [&](const UPlayerDataComponent* DataComponent)
	{
		if(DataComponent->PlayerID == InPlayerID)
		{
			return EServerCallType::ECT_ProgressComplete; //终止查找
		}
		return EServerCallType::ECT_InProgress; //继续查找
	});
	
	return false;
}

bool MethodUnit::IsFriendly(const AMobaCharacter* CharacterA, const AMobaCharacter* CharacterB)
{
	if(CharacterA && CharacterB)
	{
		return CharacterA->GetTeamType() == CharacterB->GetTeamType();
	}
	return true;
}




