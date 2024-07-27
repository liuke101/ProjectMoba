
#include "Common/CalculationUnit.h"

#include "Character/MobaCharacter.h"
#include "Table/CharacterAttribute.h"
#include "Table/SlotAttribute.h"

float CalculationUnit::GetArmor( AMobaCharacter* InTarget,  AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute)
{
	if (FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		if (FCharacterAttribute* OriginalData = InTarget->GetCharacterAttribute())
		{
			float PhysicalPenetration = InEnemyData->GetPhysicalPenetration();
			if (SlotAttribute)
			{
				PhysicalPenetration = GetAttributeValue(PhysicalPenetration,SlotAttribute->PhysicalPenetration);
			}

			return OriginalData->GetArmor() * (1.f - PhysicalPenetration / 100.f);
		}
	}

	return 0;
}

float CalculationUnit::GetMagicResistance(AMobaCharacter* InTarget,  AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute)
{
	if (FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		if (FCharacterAttribute* OriginalData = InTarget->GetCharacterAttribute())
		{
			float MagicPenetration = InEnemyData->GetMagicPenetration();
			if (SlotAttribute)
			{
				MagicPenetration = GetAttributeValue(MagicPenetration,SlotAttribute->MagicPenetration);
			}

			return OriginalData->GetMagicResistance() *(1.f - MagicPenetration / 100.f);
		}
	}

	return 0;
}

float CalculationUnit::GetAttack( AMobaCharacter* InOriginal, const FSlotAttribute* SlotAttribute)
{
	if (FCharacterAttribute* OriginalData = InOriginal->GetCharacterAttribute())
	{
		float PhysicalAttack = OriginalData->GetPhysicalAttack();
		float CriticalStrike = OriginalData->GetCriticalRate();
		if (SlotAttribute)
		{
			PhysicalAttack = GetAttributeValue(PhysicalAttack, SlotAttribute->PhysicalAttack);
			CriticalStrike = GetAttributeValue(CriticalStrike, SlotAttribute->CriticalRate);
		}

		return PhysicalAttack * (1 + CriticalStrike) + OriginalData->Level;
	}

	return 0;
}

float CalculationUnit::GetPhysicalDamage( AMobaCharacter* InTarget,  AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute)
{
	if (FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		return GetAttack(InEnemy, SlotAttribute) * (100 / (100 + GetArmor(InTarget, InEnemy, SlotAttribute)));
	}

	return 0;
}

float CalculationUnit::GetMagicDamage( AMobaCharacter* InTarget,  AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute)
{
	if (FCharacterAttribute* InEnemyData = InEnemy->GetCharacterAttribute())
	{
		if (FCharacterAttribute* OriginalData = InTarget->GetCharacterAttribute())
		{
			float MagicAttackBaseValue = InEnemyData->GetMagicAttack();
			if (SlotAttribute)
			{
				MagicAttackBaseValue = GetAttributeValue(MagicAttackBaseValue,SlotAttribute->MagicAttack);
			}

			return MagicAttackBaseValue * (100 / (100 + GetMagicResistance(InTarget, InEnemy, SlotAttribute)));
		}
	}

	return 0;
}

float CalculationUnit::GetTotalDamage( AMobaCharacter* InTarget,  AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute)
{
	return GetPhysicalDamage(InTarget, InEnemy, SlotAttribute) + GetMagicDamage(InTarget, InEnemy, SlotAttribute);
}

float CalculationUnit::GetAttributeValue(float BaseAttributeValue, const FSlotAttributeValue& InSlotAttribute)
{

	if(InSlotAttribute.ValueType == ESlotAttributeValueType::ESAVT_Value)
	{
		if(InSlotAttribute.GainType == ESlotAttributeGainType::ESAGT_Add)
		{
			BaseAttributeValue += InSlotAttribute.Value;
		}
		else if(InSlotAttribute.GainType == ESlotAttributeGainType::ESAGT_Substract)
		{
			BaseAttributeValue += -InSlotAttribute.Value;
		}
	}
	else if(InSlotAttribute.ValueType == ESlotAttributeValueType::ESAVT_Percent)
	{
		if(InSlotAttribute.GainType == ESlotAttributeGainType::ESAGT_Add)
		{
			if(InSlotAttribute.Value >= 0 && InSlotAttribute.Value <= 1)
			{
				BaseAttributeValue += InSlotAttribute.Value * BaseAttributeValue;
			}
			else if(InSlotAttribute.Value > 1.0f)
			{
				BaseAttributeValue*=2.0f;
			}
		}
		else if(InSlotAttribute.GainType == ESlotAttributeGainType::ESAGT_Substract)
		{
			BaseAttributeValue += -InSlotAttribute.Value * BaseAttributeValue;
		}
	}

	if(BaseAttributeValue < 0.0f)
	{
		BaseAttributeValue = 0.0f;
	}
	
	return BaseAttributeValue;
}
