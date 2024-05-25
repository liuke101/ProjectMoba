// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameMode.h"

#include "ThreadManage.h"
#include "Character/MobaCharacter.h"
#include "Character/Tool/CharacterSpawnPoint.h"
#include "Common/MethodUnit.h"
#include "Game/MobaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Table/CharacterAsset.h"

AMobaGameMode::AMobaGameMode()
{
	/** 开启tick */
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMobaGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}


void AMobaGameMode::BeginPlay()
{
	Super::BeginPlay();

	/** GameMode仅存在于服务器, 不需要进行LocalRole判断 */

	// 延迟在服务器上生成小兵
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMobaGameMode::SpawnMinions, 2.f, true);
}


void AMobaGameMode::SpawnMinions()
{
	if(SpawnTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}

	//在服务器上生成小兵
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		AActor* CharacterSpawnPoint = UGameplayStatics::GetActorOfClass(GetWorld(),ACharacterSpawnPoint::StaticClass());
		int32 CharacterID = 11110;  //暂时写死
	
		UClass* DefaultCharacterClass = nullptr;
		if(const FCharacterAsset* CharacterAsset = MobaGameState->GetCharacterAssetFromCharacterID(CharacterID))
		{
			DefaultCharacterClass = CharacterAsset->CharacterClass;
		}
	 		
		if(DefaultCharacterClass)
		{
			if(AMobaCharacter* MobaCharacter = GetWorld()->SpawnActor<AMobaCharacter>(DefaultCharacterClass, CharacterSpawnPoint->GetActorLocation(), CharacterSpawnPoint->GetActorRotation()))
			{
				int64 PlayerID = 123456; //暂时写死
				
				if(PlayerID!= INDEX_NONE)
				{
					MobaCharacter->RegisterCharacterOnServer(PlayerID, CharacterID, ETeamType::ETT_None, ECharacterType::ECT_None);
				}
			}
		}
	}
}
