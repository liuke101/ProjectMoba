// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameState.h"

AMobaGameState::AMobaGameState()
{

}

const TArray<FCharacterTable*>* AMobaGameState::GetCharacterTables()
{
	if(CacheCharacterTables.IsEmpty())
	{
		if(CharacterTablePtr)
		{
			CharacterTablePtr->GetAllRows(TEXT("Character Table"), CacheCharacterTables);
		}
	}

	return &CacheCharacterTables;
}

const FCharacterTable* AMobaGameState::GetCharacterTable(const int64& InID)
{
	for(auto &Table : *GetCharacterTables())
	{
		if(Table->CharacterID == InID)
		{
			return Table;
		}
	}

	return nullptr;
}

