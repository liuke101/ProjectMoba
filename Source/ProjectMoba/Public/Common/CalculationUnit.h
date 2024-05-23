// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class AMobaCharacter;

namespace CalculationUnit
{
	float GetPhysicalArmor( AMobaCharacter* InTarget, AMobaCharacter* InEnemy);
	float GetMagicArmor( AMobaCharacter* InTarget, AMobaCharacter* InEnemy);
	float GetAttack(AMobaCharacter* InOriginal);

	/** 对方给我造成的伤害 */
	float GetPhysicalDamage( AMobaCharacter* InTarget, AMobaCharacter* InEnemy);

	/** 对方给我造成的伤害 */
	float GetMagicDamage(AMobaCharacter* InTarget, AMobaCharacter* InEnemy);
	
	/** 对方给我造成的伤害 */
	float GetTotalDamage( AMobaCharacter* InTarget, AMobaCharacter* InEnemy);
}
