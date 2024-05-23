// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameMode.h"
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

	/** 每帧更新角色属性, 也可以写在Character类的Tick中 */
	/** GameMode仅存在于服务器, 不需要进行LocalRole判断 */
	// if(MobaGameState)
	// {
	// 	ServerCallAllCharacterAI<AMobaCharacter>([&](const AMobaCharacter* MobaCharacter)
	// 	{
	// 		
	// 	});
	// }
}

void AMobaGameMode::BeginPlay()
{
	Super::BeginPlay();
}
