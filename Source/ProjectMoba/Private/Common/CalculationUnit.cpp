
#include "Common/CalculationUnit.h"

#include "Character/MobaCharacter.h"
#include "Table/CharacterAttribute.h"

float CalculationUnit::GetPhysicalArmor(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy)
{
	if (const FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		if (const FCharacterAttribute* OriginalData = InTarget->GetCharacterAttribute())
		{
			return OriginalData->PhysicalArmor * (1.f - InEnemyData->PhysicalPenetration / 100.f);
		}
	}

	return 0;
}

float CalculationUnit::GetMagicArmor(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy)
{
	if (const FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		if (const FCharacterAttribute* OriginalData = InTarget->GetCharacterAttribute())
		{
			return OriginalData->MagicArmor *(1.f - InEnemyData->MagicPenetration / 100.f);
		}
	}

	return 0;
}

float CalculationUnit::GetAttack(const AMobaCharacter* InOriginal)
{
	if (const FCharacterAttribute* OriginalData = InOriginal->GetCharacterAttribute())
	{
		return OriginalData->PhysicalAttack* (1 + OriginalData->CriticalRate) + OriginalData->Level;
	}

	return 0;
}

float CalculationUnit::GetPhysicalDamage(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy)
{
	if (const FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		return GetAttack(InEnemy) / ((GetPhysicalArmor(InTarget, InEnemy) / 100) - InEnemyData->Level * 2);
	}

	return 0;
}

float CalculationUnit::GetMagicDamage(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy)
{
	if (const FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		if (const FCharacterAttribute* OriginalData = InTarget->GetCharacterAttribute())
		{
			return InEnemyData->MagicPenetration / ((GetMagicArmor(InTarget, InEnemy) / 70) - InEnemyData->Level);
		}
	}

	return 0;
}

float CalculationUnit::GetTotalDamage(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy)
{
	return GetPhysicalDamage(InTarget, InEnemy) + GetMagicDamage(InTarget, InEnemy);
}
