// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "CharacterAttribute.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAttribute : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterAttribute();

	UPROPERTY()
	int32 Level; //等级

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MaxHealth; //最大生命值

	UPROPERTY()
	float CurrentHealth; //当前生命值
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MaxMana; //最大魔法值

	UPROPERTY()
	float CurrentMana; //当前魔法值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MaxAnger; //最大怒气值

	UPROPERTY()
	float CurrentAnger; //当前怒气值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float PhysicalAttack; //物理攻击

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float PhysicalArmor; //物理护甲

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float PhysicalPenetration; //物理穿透

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MagicAttack; //魔法攻击

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MagicArmor; //魔法护甲

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MagicPenetration; //魔法穿透
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float WalkSpeed; //移动速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float AttackSpeed; //攻击速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MaxEXP; //最大经验值

	UPROPERTY()
	float CurrentEXP; //当前经验值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float AttackRange; //攻击范围

	void InitAttribute();

	float GetHealthPercent() const;
	float GetManaPercent() const;
	float GetEXPPercent() const;
	float GetAngerPercent() const;
};