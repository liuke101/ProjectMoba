// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameState.h"

AMobaGameState::AMobaGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterTableObject(TEXT("DataTable'/Game/DataTable/CharacterTable.CharacterTable'"));
	
	if (CharacterTableObject.Succeeded())
	{
		CharacterTablePtr = CharacterTableObject.Object;
	}
}
//
// const FCharacterTable* AMobaGameState::GetCharacterTable(const int64& InID)
// {
// }
//
// const TArray<FCharacterTable*>* AMobaGameState::GetCharacterTables()
// {
// 	if(!CacheCharacterTables.IsEmpty())
// 	{
// 		if(CharacterTablePtr)
// 		{
// 			
// 		}
// 	}
// }
