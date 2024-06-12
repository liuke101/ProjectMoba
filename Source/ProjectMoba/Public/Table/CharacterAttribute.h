// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "CharacterAttribute.generated.h"

/** 积累所有SlotAttribute中的某个属性，并和基础属性相加 */
#define CALCULATE_ATTRIBUTE_VALUE(ATTRIBUTE) \
float NewValue = 0; \
if(WeakPtr_SlotAttributes.IsValid()) \
{ \
for(auto& Tmp : WeakPtr_SlotAttributes.Pin()->AttributeElements) \
{ \
if(Tmp.Value.ATTRIBUTE.GainType == ESlotAttributeGainType::ESAGT_Add) \
{ \
NewValue += Tmp.Value.ATTRIBUTE.Value; \
} \
else if(Tmp.Value.ATTRIBUTE.GainType == ESlotAttributeGainType::ESAGT_Substract) \
{ \
NewValue -= Tmp.Value.ATTRIBUTE.Value; \
} \
} \
} \
return NewValue + ATTRIBUTE;


struct FSlotAttributes;
struct FSlotAttributeValue;
struct FSlotAttribute;

USTRUCT(BlueprintType)
struct FCharacterAttribute : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterAttribute();

#pragma region 等级
	UPROPERTY()
	int32 Level; //等级
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MaxExp; //最大经验值

	UPROPERTY()
	float CurrentExp; //当前经验值
#pragma endregion 

#pragma region 属性
	

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
	float CriticalRate; //暴击率
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float AttackRange; //攻击范围

#pragma endregion 

#pragma region 击杀奖励
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reward")
	float ExpReward; //被杀后的奖励经验值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reward")
	float GoldReward; //被杀后的金币

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reward")
	int32 AddLevelID;  //获取升级数据，升级数据将与基础属性叠加

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reward")
	float Coefficient; //击杀奖励系数
	
	const FSlotAttribute* AddLevelAttribute;
#pragma endregion

	void InitAttribute();
	/** 重置 */
	void ResetAttribute();

	//注册Buff，即将WeakPtr_SlotAttributes指向PlayerDataComponent的SlotAttributes实例
	void SetBuff(const TSharedRef<FSlotAttributes>& InBuff);

	void UpdateLevel();

#pragma region Getter
	float GetHealthPercent() const;
	float GetManaPercent() const;
	float GetExpPercent() const;
	
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	float GetCurrentMana() const;
	float GetMaxMana() const;
	float GetPhysicalAttack() const;
	float GetArmor() const;
	float GetPhysicalPenetration() const;
	float GetMagicAttack() const;
	float GetMagicResistance() const;
	float GetMagicPenetration() const;
	float GetWalkSpeed() const;
	float GetAttackSpeed() const;
	float GetCriticalRate() const;

	float GetExpReward() const;
	float GetGoldReward() const;
#pragma endregion 

private:
	//我们不修改CharacterAttribute类中的原始属性，而是通过若指针去获取装备属性，并在Getter函数中计算得出增加/减少后的属性值
	TWeakPtr<FSlotAttributes> WeakPtr_SlotAttributes;
};