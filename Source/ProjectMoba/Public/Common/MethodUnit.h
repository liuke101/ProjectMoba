
#pragma once

#include "CoreMinimal.h"
#include "Game/MobaGameState.h"


namespace MethodUnit
{
	AMobaGameState* GetMobaGameState(const UWorld* InWorld);

	const FCharacterAsset* GetCharacterAssetFromCharacterID(const UWorld* InWorld, int32 CharacterID);
	
	const FCharacterAttribute* GetCharacterAttributeFromCharacterID(const UWorld* InWorld, int32 CharacterID);
	const FCharacterAttribute* GetCharacterAttributeFromPlayerID(const UWorld* InWorld, int64 PlayerID);

	
}
