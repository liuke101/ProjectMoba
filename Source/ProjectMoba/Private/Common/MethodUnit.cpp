// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MethodUnit.h"

AMobaGameState* MethodUnit::GetMobaGameState(const UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetGameState<AMobaGameState>();
	}
	return nullptr;
}

const FCharacterAssetTable* MethodUnit::GetCharacterTable(const UWorld* InWorld, int32 CharacterID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAssetTable(CharacterID);
	}
	return nullptr;
}


