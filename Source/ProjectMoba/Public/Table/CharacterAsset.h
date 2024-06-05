// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "ProjectMoba/MobaType.h"
#include "CharacterAsset.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSkill
{
	GENERATED_BODY()

	FCharacterSkill();

	UPROPERTY(EditDefaultsOnly, Category = "Character Skill")
	int32 DataID = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, Category = "Character Skill")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Character Skill")
	ESkillType SkillType = ESkillType::EST_Burst;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Skill")
	TObjectPtr<UAnimMontage> SkillMontage;
};

USTRUCT(BlueprintType)
struct FCharacterAsset : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterAsset();
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TSubclassOf<APawn> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TSubclassOf<APawn> CharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TObjectPtr<UTexture2D> CharacterIcon; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	ECharacterType CharacterType = ECharacterType::ECT_None; //角色类型

	UPROPERTY(EditDefaultsOnly, Category = "Attack Montage")
	TArray<TObjectPtr<UAnimMontage>> NormalAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	FCharacterSkill W_Skill;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	FCharacterSkill E_Skill;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	FCharacterSkill R_Skill;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill Montage")
	FCharacterSkill F_Skill;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montage")
	TArray<TObjectPtr<UAnimMontage>> DeathMontages;
	
};


