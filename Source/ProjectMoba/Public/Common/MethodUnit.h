
#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacter.h"
#include "Character/MobaPawn.h"
#include "Game/MobaPlayerState.h"
#include "Kismet/GameplayStatics.h"


class UUI_Tip;
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
	
	AMobaPawn* GetMobaPawnFromPlayerID(UWorld* InWorld, int64 PlayerID); //服务端获取
	AMobaPawn* GetMobaPawn(UWorld* InWorld); //客户端获取
	AMobaCharacter* GetMobaCharacterFromPlayerID(UWorld* InWorld, int64 PlayerID);
	bool IsPlayer(UWorld* InWorld, int64 InPlayerID);
	bool IsFriendly(const AMobaCharacter* CharacterA, const AMobaCharacter* CharacterB);
	
	AMobaGameState* GetMobaGameState(const UWorld* InWorld);
	AMobaPlayerState* GetMobaPlayerStateFromPlayerID(UWorld* InWorld, int64 PlayerID); //服务端获取
	AMobaPlayerState* GetMobaPlayerState(UWorld* InWorld); //客户端获取

	const FCharacterAsset* GetCharacterAssetFromCharacterID(const UWorld* InWorld, int32 CharacterID);
	const FCharacterAsset* GetCharacterAssetFromPlayerID(const UWorld* InWorld, int64 PlayerID);
	
	const FCharacterAttribute* GetCharacterAttributeFromCharacterID(const UWorld* InWorld, int32 CharacterID);
	 FCharacterAttribute* GetCharacterAttributeFromPlayerID(const UWorld* InWorld, int64 PlayerID);

	const FSlotAttribute* GetSlotAttributeFromAnimMontage(AMobaPlayerState* PlayerState, const UAnimMontage* AnimMontage);

	/** UI */
	// ToolTip 
	FString SlotAttributeValueToString(const FSlotAttributeValue* SlotAttributeValue, const FString& AttrbuteName, const FString& ColorTag);
	FText GetAttributeDescription(const FSlotAttribute* SlotAttribute);
	
	void SetToolTip(UUI_Tip* Tip, const FSlotAttribute* SlotAttribute, const FSlotAsset* SlotAsset);

	//服务器、客户端注册SlotAttribute
	void RegisterSlotAttribute(AMobaPlayerState* PlayerState, FSlotAttribute* SlotAttribute); 
	
	
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
	void ServerCallAllMobaCharacter(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement);
	
	template<class T>
	void ServerCallAllMobaHero(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement);

	/** AI */
	template<class T>
	T* FindNearestTargetInRange(UWorld* InWorld, float Range, T* OwnerCharacter);

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
void MethodUnit::ServerCallAllMobaCharacter(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
{
	TArray<AActor*> FindActors{};
	UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), FindActors);
	for(auto Actor : FindActors)
	{
		if(T* MobaCharacter = Cast<T>(Actor))
		{
			if(InImplement(MobaCharacter) == EServerCallType::ECT_ProgressComplete)
			{
				break;
			}
		}
	}
}

template <class T>
void MethodUnit::ServerCallAllMobaHero(UWorld* InWorld, TFunction<EServerCallType(T*)> InImplement)
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

template <class T>
T* MethodUnit::FindNearestTargetInRange(UWorld* InWorld, float Range, T* OwnerCharacter)
{
	if(!InWorld || !OwnerCharacter || Range == 0) return nullptr;

	T* ClosestEnemy = nullptr;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), FoundActors);
	float TempDistance =  99999.0f;

	//获取最近的距离敌人
	for(auto& Actor : FoundActors)
	{
		if(Actor != OwnerCharacter) //排除自己
		{
			if(T* TargetCharacter = Cast<T>(Actor))
			{
				if(TargetCharacter->IsDead() || IsFriendly(OwnerCharacter,TargetCharacter)) continue;
			
				float Distance = FVector::Dist(Actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
				
				if(Distance <= TempDistance)
				{
					TempDistance = Distance;
					if(TempDistance <= Range)
					{
						ClosestEnemy = TargetCharacter;
					}
				}
			}
		}
	}

	return ClosestEnemy;
}




