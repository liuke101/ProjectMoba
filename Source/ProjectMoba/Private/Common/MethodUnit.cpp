// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MethodUnit.h"

#include "Component/PlayerDataComponent.h"
#include "Game/MobaGameState.h"

AMobaGameState* MethodUnit::GetMobaGameState(const UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetGameState<AMobaGameState>();
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




