// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class AMobaCharacter;

namespace CalculationUnit
{
	float GetPhysicalArmor(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy);
	float GetMagicArmor(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy);
	float GetAttack(const AMobaCharacter* InOriginal);

	/** 对方给我造成的伤害 */
	float GetPhysicalDamage(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy);

	/** 对方给我造成的伤害 */
	float GetMagicDamage(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy);
	
	/** 对方给我造成的伤害 */
	float GetTotalDamage(const AMobaCharacter* InTarget, const AMobaCharacter* InEnemy);
}
