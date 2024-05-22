// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "CharacterAsset.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAsset : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterAsset();
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TSubclassOf<APawn> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Montage")
	TArray<TObjectPtr<UAnimMontage>> NormalAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	TObjectPtr<UAnimMontage> W_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	TObjectPtr<UAnimMontage> E_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	TObjectPtr<UAnimMontage> R_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	TObjectPtr<UAnimMontage> F_SkillMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	TObjectPtr<UAnimMontage> Space_SkillMontage;
	
};


