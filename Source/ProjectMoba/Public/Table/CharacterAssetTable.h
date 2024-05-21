// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "CharacterAssetTable.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAssetTable : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterAssetTable();
	
	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TSubclassOf<APawn> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TArray<TObjectPtr<UAnimMontage>> NormalAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TObjectPtr<UAnimMontage> W_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TObjectPtr<UAnimMontage> E_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TObjectPtr<UAnimMontage> R_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TObjectPtr<UAnimMontage> F_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TObjectPtr<UAnimMontage> Space_SkillMontage;
	
};


