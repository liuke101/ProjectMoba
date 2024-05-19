// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameState.h"

AMobaGameState::AMobaGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterTableObject(TEXT("/Game/Table/DT_CharacterTable"));
	
	if (CharacterTableObject.Succeeded())
	{
		CharacterTablePtr = CharacterTableObject.Object;
	}
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
	for(auto &Tmp : *GetCharacterTables())
	{
		if(Tmp->CharacterID == InID)
		{
			return Tmp;
		}
	}

	return nullptr;
}

