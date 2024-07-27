// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class AMobaCharacter;
struct FSlotAttribute;
struct FSlotAttributeValue;

namespace CalculationUnit
{
	float GetArmor( AMobaCharacter* InTarget, AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute = nullptr);
	float GetMagicResistance( AMobaCharacter* InTarget, AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute = nullptr);
	float GetAttack(AMobaCharacter* InOriginal, const FSlotAttribute* SlotAttribute = nullptr);

	/** 对方给我造成的伤害 */
	float GetPhysicalDamage( AMobaCharacter* InTarget, AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute = nullptr);

	/** 对方给我造成的伤害 */
	float GetMagicDamage(AMobaCharacter* InTarget, AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute = nullptr);
	
	/** 对方给我造成的伤害 */
	float GetTotalDamage( AMobaCharacter* InTarget, AMobaCharacter* InEnemy, const FSlotAttribute* SlotAttribute = nullptr);

	/** 获取基础值与SlotAttributeValue计算后的值 */
	float GetAttributeValue(float BaseAttributeValue, const FSlotAttributeValue& InSlotAttribute);
}
