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
	MaxHealth = GetMaxHealth();
	CurrentHealth = MaxHealth;
	MaxMana = GetMaxMana();
	CurrentMana = MaxMana;
	PhysicalAttack = 0.0f;
	Armor = 0.0f;
	PhysicalPenetration = 0.0f;
	MagicAttack = 0.0f;
	MagicResistance = 0.0f;
	MagicPenetration = 0.0f;
	WalkSpeed = 300.0f;
	AttackSpeed = 0.5f;
	CriticalRate = 0.0f;
	AttackRange = 200.0f;
	MaxExp = 100.0f;
	CurrentExp = 0.0f;
}

void FCharacterAttribute::ResetAttribute()
{
	CurrentHealth = GetMaxHealth();
	CurrentMana = GetMaxMana();
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

void FCharacterAttribute::SetBuff(const TSharedRef<FSlotAttributes>& InBuff)
{
	WeakPtr_SlotAttributes = InBuff;
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

float FCharacterAttribute::GetPhysicalAttack() const
{
	CALCULATE_ATTRIBUTE_VALUE(PhysicalAttack);
}
