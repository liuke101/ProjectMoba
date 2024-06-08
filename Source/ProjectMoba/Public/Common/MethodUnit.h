
#pragma once

#include "CoreMinimal.h"
#include "Character/MobaPawn.h"
#include "Game/MobaPlayerState.h"


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
	AMobaPlayerState* GetMobaPlayerStateFromPlayerID(UWorld* InWorld, int64 PlayerID);

	const FCharacterAsset* GetCharacterAssetFromCharacterID(const UWorld* InWorld, int32 CharacterID);
	const FCharacterAsset* GetCharacterAssetFromPlayerID(const UWorld* InWorld, int64 PlayerID);
	
	const FCharacterAttribute* GetCharacterAttributeFromCharacterID(const UWorld* InWorld, int32 CharacterID);
	 FCharacterAttribute* GetCharacterAttributeFromPlayerID(const UWorld* InWorld, int64 PlayerID);

	AMobaCharacter* GetMobaCharacterFromPlayerID(UWorld* InWorld, int64 PlayerID);
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
	void ServerCallAllHero(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement);

}

//模板声明和实现要写在一个文件中，否则 Error LNK2019
template <class T>
void MethodUnit::ServerCallAllPlayerController(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
{
	// 令所有的PlayerController执行 void InImplement(T*) 函数
	for(auto It = InWorld->GetPlayerControllerIterator(); It; ++It)
	{
		if(T* PlayerController = Cast<T>(*It))
		{
			if(InImplement(PlayerController) == EServerCallType::ECT_ProgressComplete)
			{
				break;  //如果完成，断开连接
			}
		}
	}
}

template <class T>
void MethodUnit::ServerCallAllPlayerState(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
{
	// 令所有的PlayerController执行 Lambda 函数
	// 通过该 Lambda 函数调用所有的PlayerState 执行 InImplement 函数
	ServerCallAllPlayerController<APlayerController>(InWorld, [&](const APlayerController* PlayerController)->EServerCallType
	{
		if(T* PlayerState = PlayerController->GetPlayerState<T>())
		{
			return InImplement(PlayerState);
		}

		return EServerCallType::ECT_ProgressComplete;
	});
}

template <class T>
void MethodUnit::ServerCallAllPlayerDataComponent(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
{
	ServerCallAllPlayerState<AMobaPlayerState>(InWorld, [&](const AMobaPlayerState* MobaPlayerState)->EServerCallType
	{
		if(T* DataComponent = Cast<T>(MobaPlayerState->GetPlayerDataComponent()))
		{
			return InImplement(DataComponent);
		}
		
		return EServerCallType::ECT_ProgressComplete;
	});
}

template <class T>
void MethodUnit::ServerCallAllPlayer(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
{
	ServerCallAllPlayerController<APlayerController>(InWorld, [&](const APlayerController* PlayerController)->EServerCallType
	{
		if(T* Pawn = PlayerController->GetPawn<T>())
		{
			return InImplement(Pawn);
		}
		
		return EServerCallType::ECT_ProgressComplete;
	});
}

template <class T>
void MethodUnit::ServerCallAllHero(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
{
	ServerCallAllPlayer<AMobaPawn>(InWorld, [&](const AMobaPawn* MobaPawn)->EServerCallType
	{
		if(T* Hero = Cast<T>(MobaPawn->GetControlledMobaHero()))
		{
			return InImplement(Hero);
		}
		return EServerCallType::ECT_ProgressComplete;
	});
}




