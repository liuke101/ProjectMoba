// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MethodUnit.h"

#include "Character/MobaPawn.h"
#include "Component/PlayerDataComponent.h"
#include "Game/MobaGameState.h"
#include "Game/MobaPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"

AMobaGameState* MethodUnit::GetMobaGameState(const UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetGameState<AMobaGameState>();
	}
	return nullptr;
}

const FCharacterAsset* MethodUnit::GetCharacterAssetFromCharacterID(const UWorld* InWorld, const int32 CharacterID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAssetFromCharacterID(CharacterID);
	}
	return nullptr;
}

const FCharacterAsset* MethodUnit::GetCharacterAssetFromPlayerID(const UWorld* InWorld, int64 PlayerID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAssetFromPlayerID(PlayerID);
	}
	return nullptr;
}


const FCharacterAttribute* MethodUnit::GetCharacterAttributeFromCharacterID(const UWorld* InWorld, const int32 CharacterID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAttributeFromCharacterID(CharacterID);
	}
	return nullptr;
}


FCharacterAttribute* MethodUnit::GetCharacterAttributeFromPlayerID(const UWorld* InWorld, int64 PlayerID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAttributeFromPlayerID(PlayerID);
	}
	return nullptr;
}

bool MethodUnit::IsPlayer(UWorld* InWorld, int64 InPlayerID)
{
	ServerCallAllPlayerDataComponent<UPlayerDataComponent>(InWorld, [&](const UPlayerDataComponent* DataComponent)
	{
		if(DataComponent->PlayerID == InPlayerID)
		{
			return EServerCallType::ECT_ProgressComplete; //终止查找
		}
		return EServerCallType::ECT_InProgress; //继续查找
	});
	
	return false;
}

template <class T>
void MethodUnit::ServerCallAllPlayerController(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
{
	// 令所有的PlayerController执行 void InImplement(T*) 函数
	for(auto it = InWorld->GetPlayerControllerIterator(); it; ++it)
	{
		if(T* PlayerController = Cast<T>(*it))
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
	ServerCallAllPlayerController<APlayerController>(InWorld, [&](const APlayerController* PlayerController)
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
	ServerCallAllPlayerState<AMobaPlayerState>(InWorld, [&](const AMobaPlayerState* MobaPlayerState)
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
	ServerCallAllPlayerController<APlayerController>(InWorld, [&](const APlayerController* PlayerController)
	{
		if(T* Pawn = PlayerController->GetPawn<T>())
		{
			return InImplement(Pawn);
		}
		
		return EServerCallType::ECT_ProgressComplete;
	});
}

template <class T>
void MethodUnit::ServerCallAllCharacterAI(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
{
	ServerCallAllPlayer<AMobaPawn>(InWorld, [&](const AMobaPawn* MobaPawn)
	{
		if(T* CharacterAI = Cast<T>(MobaPawn->MobaCharacter))
		{
			return InImplement(CharacterAI);
		}
		return EServerCallType::ECT_ProgressComplete;
	});
}





