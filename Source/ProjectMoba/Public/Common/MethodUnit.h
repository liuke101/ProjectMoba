
#pragma once

#include "CoreMinimal.h"
#include "Game/MobaGameState.h"


namespace MethodUnit
{
	AMobaGameState* GetMobaGameState(const UWorld* InWorld);

	const FCharacterAssetTable* GetCharacterTable(const UWorld* InWorld, int32 CharacterID);
}
