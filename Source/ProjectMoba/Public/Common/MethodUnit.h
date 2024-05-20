
#pragma once

#include "CoreMinimal.h"
#include "Game/MobaGameState.h"


namespace MethodUnit
{
	AMobaGameState* GetMobaGameState(const UWorld* InWorld);

	const FCharacterTable* GetCharacterTable(const UWorld* InWorld, int32 CharacterID);
}
