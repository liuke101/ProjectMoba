﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	float PhysicalAttack; //物理攻击

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float Armor; //护甲

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float PhysicalPenetration; //物理穿透

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MagicAttack; //魔法攻击

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MagicResistance; //魔抗

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MagicPenetration; //魔法穿透
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float WalkSpeed; //移动速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float AttackSpeed; //攻击速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MaxExp; //最大经验值

	UPROPERTY()
	float CurrentExp; //当前经验值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float AttackRange; //攻击范围

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float CriticalRate; //暴击率

	/** 重置 */
	void ResetAttribute();

	float GetHealthPercent() const;
	float GetManaPercent() const;
	float GetExpPercent() const;
};