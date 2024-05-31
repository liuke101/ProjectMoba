// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/CharacterAttribute.h"

FCharacterAttribute::FCharacterAttribute()
{
	InitAttribute();
}

void FCharacterAttribute::InitAttribute()
{
	Level = 1;
	MaxHealth = 1000.0f;
	CurrentHealth = MaxHealth;
	MaxMana = 0.0f;
	CurrentMana = MaxMana;
	MaxAnger = 0.0f;
	CurrentAnger = MaxAnger;
	PhysicalAttack = 50.0f;
	Armor = 20.0f;
	PhysicalPenetration = 0.0f;
	MagicAttack = 60.0f;
	Armor = 20.0f;
	MagicPenetration = 0.0f;
	WalkSpeed = 300.0f;
	AttackSpeed = 0.8f;
	MaxEXP = 100.0f;
	CurrentEXP = 0.0f;
	AttackRange = 1000.0f;
	CriticalRate = 0.0f;
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

float FCharacterAttribute::GetEXPPercent() const
{
	return FMath::Clamp(CurrentEXP / MaxEXP, 0.0f, 1.0f);
}

float FCharacterAttribute::GetAngerPercent() const
{
	return FMath::Clamp(CurrentAnger / MaxAnger, 0.0f, 1.0f);
}
