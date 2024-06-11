
#include "Common/CalculationUnit.h"

#include "Character/MobaCharacter.h"
#include "Table/CharacterAttribute.h"

float CalculationUnit::GetArmor( AMobaCharacter* InTarget,  AMobaCharacter* InEnemy)
{
	if (FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		if (FCharacterAttribute* OriginalData = InTarget->GetCharacterAttribute())
		{
			return OriginalData->GetArmor() * (1.f - InEnemyData->GetPhysicalPenetration() / 100.f);
		}
	}

	return 0;
}

float CalculationUnit::GetMagicResistance(AMobaCharacter* InTarget,  AMobaCharacter* InEnemy)
{
	if (FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		if (FCharacterAttribute* OriginalData = InTarget->GetCharacterAttribute())
		{
			return OriginalData->GetMagicResistance() *(1.f - InEnemyData->GetMagicPenetration() / 100.f);
		}
	}

	return 0;
}

float CalculationUnit::GetAttack( AMobaCharacter* InOriginal)
{
	if (FCharacterAttribute* OriginalData = InOriginal->GetCharacterAttribute())
	{
		return OriginalData->GetPhysicalAttack()* (1 + OriginalData->GetCriticalRate()) + OriginalData->Level;
	}

	return 0;
}

float CalculationUnit::GetPhysicalDamage( AMobaCharacter* InTarget,  AMobaCharacter* InEnemy)
{
	if (FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		return GetAttack(InEnemy) / ((GetArmor(InTarget, InEnemy) / 100) - InEnemyData->Level * 2);
	}

	return 0;
}

float CalculationUnit::GetMagicDamage( AMobaCharacter* InTarget,  AMobaCharacter* InEnemy)
{
	if (FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		return InEnemyData->GetMagicPenetration() / ((GetMagicResistance(InTarget, InEnemy) / 70) - InEnemyData->Level);
	}

	return 0;
}

float CalculationUnit::GetTotalDamage( AMobaCharacter* InTarget,  AMobaCharacter* InEnemy)
{
	return GetPhysicalDamage(InTarget, InEnemy) + GetMagicDamage(InTarget, InEnemy);
}
