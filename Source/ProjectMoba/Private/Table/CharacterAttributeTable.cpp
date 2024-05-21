// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/CharacterAttributeTable.h"

FCharacterAttributeTable::FCharacterAttributeTable()
{
	InitAttributes();
}

void FCharacterAttributeTable::InitAttributes()
{
	Level = 1;
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	MaxMana = 100.0f;
	CurrentMana = MaxMana;
	MaxAnger = 100.0f;
	CurrentAnger = MaxAnger;
	PhysicalAttack = 50.0f;
	PhysicalArmor = 20.0f;
	PhysicalPenetration = 0.0f;
	MagicAttack = 60.0f;
	MagicArmor = 20.0f;
	MagicPenetration = 0.0f;
	WalkSpeed = 300.0f;
	AttackSpeed = 0.8f;
	MaxEXP = 100.0f;
	CurrentEXP = 0.0f;
	
}
