// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/CharacterAttribute.h"
#include "Component/PlayerDataComponent.h"
#include "ProjectMoba/MobaMacro.h"

FCharacterAttribute::FCharacterAttribute()
{
	InitAttribute();
}

void FCharacterAttribute::InitAttribute()
{
	Level = 1;
	MaxExp = 100.0f;
	CurrentExp = 0.0f;
	
	MaxHealth = GetMaxHealth();
	CurrentHealth = MaxHealth;
	MaxMana = GetMaxMana();
	CurrentMana = MaxMana;
	PhysicalAttack = 50.0f;
	Armor = 0.0f;
	PhysicalPenetration = 0.0f;
	MagicAttack = 50.0f;
	MagicResistance = 0.0f;
	MagicPenetration = 0.0f;
	WalkSpeed = 300.0f;
	AttackSpeed = 0.5f;
	CriticalRate = 0.0f;
	AttackRange = 200.0f;

	ExpReward = 20.0f;
	GoldReward = 20.0f;
	Coefficient = 0.1f;
	AddLevelID = INDEX_NONE;
}

void FCharacterAttribute::ResetAttribute()
{
	CurrentHealth = GetMaxHealth();
	CurrentMana = GetMaxMana();
}

void FCharacterAttribute::SetBuff(const TSharedRef<FSlotAttributes>& InBuff)
{
	WeakPtr_SlotAttributes = InBuff;
}

void FCharacterAttribute::UpdateLevel()
{
	if (AddLevelAttribute)
	{
		Level ++;
		MaxHealth += (Level - 1) * AddLevelAttribute->MaxHealth.Value * Coefficient;
		MaxMana += (Level - 1) * AddLevelAttribute->MaxMana.Value * Coefficient;
		// 设置为升级满血满蓝
		// CurrentHealth = MaxHealth;
		// CurrentMana = MaxMana;
		
		PhysicalAttack += (Level - 1) * AddLevelAttribute->PhysicalAttack.Value * Coefficient;
		Armor += (Level - 1) * AddLevelAttribute->Armor.Value * Coefficient;
		PhysicalPenetration += (Level - 1) * AddLevelAttribute->PhysicalPenetration.Value * Coefficient;
		
		MagicAttack += (Level - 1) * AddLevelAttribute->MagicAttack.Value * Coefficient;
		MagicResistance += (Level - 1) * AddLevelAttribute->MagicResistance.Value * Coefficient;
		MagicPenetration += (Level - 1) * AddLevelAttribute->MagicPenetration.Value * Coefficient;
		
		WalkSpeed += (Level - 1) * AddLevelAttribute->WalkSpeed.Value * Coefficient;
		AttackSpeed += (Level - 1) * AddLevelAttribute->AttackSpeed.Value * Coefficient;
		CriticalRate += (Level - 1) * AddLevelAttribute->CriticalRate.Value * Coefficient;
		
		MaxExp += (Level - 1) * AddLevelAttribute->MaxExp.Value * Coefficient;
		ExpReward += (Level - 1) * AddLevelAttribute->ExpReward.Value * Coefficient;
	}
}

bool FCharacterAttribute::CostHealth(float HealthCost)
{
	if(HealthCost <= 0.0f)
	{
		return false;
	}
	
	if(CurrentHealth >= HealthCost)
	{
		CurrentHealth -= HealthCost;
		return true;
	}
	return false;
}

bool FCharacterAttribute::CostMana(float ManaCost)
{
	if(ManaCost <= 0.0f)
	{
		return false;
	}
	
	if(CurrentMana >= ManaCost)
	{
		CurrentMana -= ManaCost;
		return true;
	}
	return false;
}


float FCharacterAttribute::GetHealthPercent() const
{
	return FMath::Clamp(CurrentHealth / GetMaxHealth(), 0.0f, 1.0f);
}

float FCharacterAttribute::GetManaPercent() const
{
	return FMath::Clamp(CurrentMana / GetMaxMana(), 0.0f, 1.0f);
}

float FCharacterAttribute::GetExpPercent() const
{
	return FMath::Clamp(CurrentExp / MaxExp, 0.0f, 1.0f);
}

float FCharacterAttribute::GetCurrentHealth() const
{
	CALCULATE_ATTRIBUTE_VALUE(CurrentHealth);
}

float FCharacterAttribute::GetCurrentMana() const
{
	CALCULATE_ATTRIBUTE_VALUE(CurrentMana);
}

float FCharacterAttribute::GetMaxHealth() const
{
	CALCULATE_ATTRIBUTE_VALUE(MaxHealth);
}

float FCharacterAttribute::GetMaxMana() const
{
	CALCULATE_ATTRIBUTE_VALUE(MaxMana);
}

float FCharacterAttribute::GetArmor() const
{
	CALCULATE_ATTRIBUTE_VALUE(Armor);
}

float FCharacterAttribute::GetPhysicalPenetration() const
{
	CALCULATE_ATTRIBUTE_VALUE(PhysicalPenetration);
}

float FCharacterAttribute::GetPhysicalAttack() const
{
	CALCULATE_ATTRIBUTE_VALUE(PhysicalAttack);
}

float FCharacterAttribute::GetMagicAttack() const
{
	CALCULATE_ATTRIBUTE_VALUE(MagicAttack);
}

float FCharacterAttribute::GetMagicResistance() const
{
	CALCULATE_ATTRIBUTE_VALUE(MagicResistance);
}

float FCharacterAttribute::GetMagicPenetration() const
{
	CALCULATE_ATTRIBUTE_VALUE(MagicPenetration);
}

float FCharacterAttribute::GetWalkSpeed() const
{
	CALCULATE_ATTRIBUTE_VALUE(WalkSpeed);
}

float FCharacterAttribute::GetAttackSpeed() const
{
	CALCULATE_ATTRIBUTE_VALUE(AttackSpeed);
}

float FCharacterAttribute::GetCriticalRate() const
{
	CALCULATE_ATTRIBUTE_VALUE(CriticalRate);
}

float FCharacterAttribute::GetExpReward() const
{
	return static_cast<float>(Level) * (ExpReward + 0.5f);
}

float FCharacterAttribute::GetGoldReward() const
{
	return GoldReward;
}

