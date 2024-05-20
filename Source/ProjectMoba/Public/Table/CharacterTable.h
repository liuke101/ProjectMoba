// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterTable.generated.h"

USTRUCT(BlueprintType)
struct FCharacterTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterTable();

	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
	int64 CharacterID;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
	TSubclassOf<APawn> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
	TArray<TObjectPtr<UAnimMontage>> NormalAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
	TObjectPtr<UAnimMontage> W_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
	TObjectPtr<UAnimMontage> E_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
	TObjectPtr<UAnimMontage> R_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
	TObjectPtr<UAnimMontage> F_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterTable")
	TObjectPtr<UAnimMontage> Space_SkillMontage;
	
};


