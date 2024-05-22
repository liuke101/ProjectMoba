// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameMode.h"

#include "Character/MobaCharacter.h"
#include "Character/MobaPawn.h"
#include "Game/MobaGameState.h"


AMobaGameMode::AMobaGameMode()
{
	/** 开启tick */
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMobaGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(MobaGameState == nullptr)
	{
		MobaGameState = GetGameState<AMobaGameState>();
	}

	/** 每帧更新角色属性 */
	/** GameMode仅存在于服务器, 不需要进行LocalRole判断 */
	if(MobaGameState)
	{
		ServerCallAllCharacterAI<AMobaCharacter>([&](const AMobaCharacter* MobaCharacter)
		{
			MobaGameState->UpdateCharacterLocation(MobaCharacter->GetPlayerID(), MobaCharacter->GetActorLocation());
		});
	}
}

void AMobaGameMode::BeginPlay()
{
	Super::BeginPlay();
}

template <class T>
void AMobaGameMode::ServerCallAllPlayerController(TFunction<void(T*)> InImplement)
{
	for(auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		if(T* PlayerController = Cast<T>(*it))
		{
			InImplement(PlayerController);
		}
	}
}

template <class T>
void AMobaGameMode::ServerCallAllPlayer(TFunction<void(T*)> InImplement)
{
	ServerCallAllPlayerController<APlayerController>([&](const APlayerController* PlayerController)
	{
		if(T* Pawn = Cast<T>(PlayerController->GetPawn()))
		{
			InImplement(Pawn);
		}
	});
}

template <class T>
void AMobaGameMode::ServerCallAllCharacterAI(TFunction<void(T*)> InImplement)
{
	ServerCallAllPlayer<AMobaPawn>([&](AMobaPawn* MobaPawn)
	{
		if(T* CharacterAI = Cast<T>(MobaPawn->MobaCharacter))
		{
			InImplement(CharacterAI);
		}
	});
}
