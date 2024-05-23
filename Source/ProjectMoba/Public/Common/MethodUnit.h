
#pragma once

#include "CoreMinimal.h"


class AMobaPlayerState;
struct FCharacterAttribute;
struct FCharacterAsset;
class AMobaGameState;

namespace MethodUnit
{
	UENUM()
	enum class EServerCallType : uint8
	{
		ECT_InProgress,
		ECT_ProgressComplete
	};
	
	AMobaGameState* GetMobaGameState(const UWorld* InWorld);

	const FCharacterAsset* GetCharacterAssetFromCharacterID(const UWorld* InWorld, int32 CharacterID);
	const FCharacterAsset* GetCharacterAssetFromPlayerID(const UWorld* InWorld, int64 PlayerID);
	
	const FCharacterAttribute* GetCharacterAttributeFromCharacterID(const UWorld* InWorld, int32 CharacterID);
	 FCharacterAttribute* GetCharacterAttributeFromPlayerID(const UWorld* InWorld, int64 PlayerID);

	bool IsPlayer(UWorld* InWorld, int64 InPlayerID);
	
	/** 服务器调用 */
	template<class T>
	void ServerCallAllPlayerController(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement);

	template<class T>
	void ServerCallAllPlayerState(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement);

	template<class T>
	void ServerCallAllPlayerDataComponent(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement);

	template<class T>
	void ServerCallAllPlayer(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement);

	template<class T>
	void ServerCallAllCharacterAI(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement);
	
}


