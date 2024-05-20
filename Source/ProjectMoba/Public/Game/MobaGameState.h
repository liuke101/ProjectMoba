// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Table/CharacterTable.h"
#include "MobaGameState.generated.h"

struct FCharacterTable;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API AMobaGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMobaGameState();

	const TArray<FCharacterTable*>* GetCharacterTables();
	const FCharacterTable* GetCharacterTable(const int64& InID);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
    TObjectPtr<UDataTable> CharacterTablePtr;
	
private:
	TArray<FCharacterTable*> CacheCharacterTables; //因为FCharacterTable有反射， 所以不能使用前置声明，直接include
};
