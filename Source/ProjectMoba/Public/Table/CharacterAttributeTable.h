// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "CharacterAttributeTable.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAttributeTable : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterAttributeTable();

	UPROPERTY()
	int32 Level; //等级

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float MaxHealth; //最大生命值

	UPROPERTY()
	float CurrentHealth; //当前生命值
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float MaxMana; //最大魔法值

	UPROPERTY()
	float CurrentMana; //当前魔法值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float MaxAnger; //最大怒气值

	UPROPERTY()
	float CurrentAnger; //当前怒气值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float PhysicalAttack; //物理攻击

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float PhysicalArmor; //物理护甲

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float PhysicalPenetration; //物理穿透

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float MagicAttack; //魔法攻击

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float MagicArmor; //魔法护甲

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float MagicPenetration; //魔法穿透
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float WalkSpeed; //移动速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float AttackSpeed; //攻击速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba DataTable")
	float MaxEXP; //最大经验值

	UPROPERTY()
	float CurrentEXP; //当前经验值

	void InitAttributes();
};