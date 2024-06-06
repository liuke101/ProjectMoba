// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/CharacterAttribute.h"

FCharacterAttribute::FCharacterAttribute()
	: Level(1)
	, MaxHealth(1000.0f)
	, CurrentHealth(MaxHealth)
	, MaxMana(0.0f)
	, CurrentMana(MaxMana)
	, PhysicalAttack(50.0f)
	, Armor(20.0f)
	, PhysicalPenetration(0.0f)
	, MagicAttack(60.0f)
	, MagicResistance(20.0f)
	, MagicPenetration(0.0f)
	, WalkSpeed(300.0f)
	, AttackSpeed(0.8f)
	, MaxExp(100.0f)
	, CurrentExp(0.0f)
	, AttackRange(1000.0f)
	, CriticalRate(0.0f)
{
}


void FCharacterAttribute::ResetAttribute()
{
	CurrentHealth = MaxHealth;
	CurrentMana = MaxMana;
}

float FCharacterAttribute::GetHealthPercent() const
{
	return FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
}

float FCharacterAttribute::GetManaPercent() const
{
	return FMath::Clamp(CurrentMana / MaxMana, 0.0f, 1.0f);
}

float FCharacterAttribute::GetExpPercent() const
{
	return FMath::Clamp(CurrentExp / MaxExp, 0.0f, 1.0f);
}
