﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "SlotAttribute.generated.h"


/** 属性类型 */
UENUM(BlueprintType)
enum class ESlotAttributeType : uint8
{
	ESAT_Burst UMETA(DisplayName = "Burst"), //瞬发
	ESAT_Continuous UMETA(DisplayName = "Continuous"), //持续, 视为BUF
	ESAT_Permanent UMETA(DisplayName = "Permanent"), //永久, 技能和装备都要选择该类型，否则会被当做消耗品
};

/** 增益类型 */
UENUM(BlueprintType)
enum class ESlotAttributeGainType : uint8
{
	ESAGT_Add UMETA(DisplayName = "Add"), //+
	ESAGT_Substract UMETA(DisplayName = "Substract"), //-
};

/** 值类型 */
UENUM(BlueprintType)
enum class ESlotAttributeValueType : uint8
{
	ESAVT_Value UMETA(DisplayName = "Value"), //值
	ESAVT_Percent UMETA(DisplayName = "Percent"), //百分比
};

USTRUCT(BlueprintType)
struct FSlotAttributeValue
{
	GENERATED_BODY()
	
	FSlotAttributeValue();

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	ESlotAttributeGainType GainType;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribut")
	ESlotAttributeValueType ValueType;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	float Value;
};


/** Slot的属性 */
USTRUCT(BlueprintType)
struct FSlotAttribute : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FSlotAttribute();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Class")
	ESlotAttributeType AttributeType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Class")
	int32 Level;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Class")
	float Time; //持续时间
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Class")
	FSlotAttributeValue CD; //道具或技能的冷却时间


	/** 属性 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue CurrentHealth; //当前生命值
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue MaxHealth; //最大生命值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue CurrentMana; //当前魔法值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue MaxMana; //最大魔法值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue PhysicalAttack; //物理攻击

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue Armor; //护甲

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue PhysicalPenetration; //物理穿透

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue MagicAttack; //魔法攻击

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue MagicResistance; //魔抗

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue MagicPenetration; //魔法穿透
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue WalkSpeed; //移动速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue AttackSpeed; //攻击速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue CriticalRate; //暴击率

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue MaxExp; //最大经验值
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Attribute")
	FSlotAttributeValue ExpReward; //被杀后的奖励经验值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
	float HealthCost; //生命消耗

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
	float ManaCost; //生命消耗

	
	//Buff 被动技能属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Class")
	int32 BuffDataID; //使用此ID查询该表用于BUff的属性

	//技能 主动技能属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Class")
	int32 ActiveSkillDataID; 

	//升级属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Class")
	int32 AddLevelDataID;

	const FSlotAttribute* BuffAttribute;
	const FSlotAttribute* ActiveSkillAttribute;
	const FSlotAttribute* AddLevelAttribute;
	
	UPROPERTY(EditDefaultsOnly, Category = "Slot Level")
	TArray<int32> LimitCondition; 
	
	void UpdateLevel();
	
};
