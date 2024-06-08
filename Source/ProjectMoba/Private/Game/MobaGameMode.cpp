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

	// 运行击杀系统
	if(AMobaGameState* MobaGameState = GetGameState<AMobaGameState>())
	{
		MobaGameState->MobaKillSystem.Tick(DeltaSeconds);
	}
}

void AMobaGameMode::BeginPlay()
{

	Super::BeginPlay();

	/** GameMode仅存在于服务器, 不需要进行LocalRole判断 */
	//在服务器上生成小兵
	SpawnMinionsOnServer();
}


void AMobaGameMode::SpawnMinionsOnServer() const
{
	
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		TArray<AActor*> SpawnPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),ACharacterSpawnPoint::StaticClass(),SpawnPoints);
		int64 PlayerID = 123456; //暂时写死
		
		for(auto& SpawnPoint : SpawnPoints)
		{
			if(ACharacterSpawnPoint* CharacterSpawnPoint = Cast<ACharacterSpawnPoint>(SpawnPoint))
			{
				//根据角色类型生成不同的角色
				int32 CharacterID = INDEX_NONE;
				if(CharacterSpawnPoint->GetCharacterType() == ECharacterType::ECT_1st_Tower)
				{
					CharacterID = 22220; //暂时写死
				}
				else if(CharacterSpawnPoint->GetCharacterType()==ECharacterType::ECT_WarriorMinion)
				{
					CharacterID = 11110;  
				}
				else if(CharacterSpawnPoint->GetCharacterType()==ECharacterType::ECT_WildMonster)
				{
					CharacterID = 33330;
				}
				else if(CharacterSpawnPoint->GetCharacterType()==ECharacterType::ECT_BossMonster)
				{
					CharacterID = 33331;
				}
				
				
				if(const FCharacterAsset* CharacterAsset = MobaGameState->GetCharacterAssetFromCharacterID(CharacterID))
				{
					if(CharacterAsset->CharacterClass)
					{
						if(AMobaCharacter* MobaCharacter = GetWorld()->SpawnActor<AMobaCharacter>(CharacterAsset->CharacterClass, CharacterSpawnPoint->GetActorLocation(), CharacterSpawnPoint->GetActorRotation()))
						{
							PlayerID++; //保证每个实例的PlayerID不同
				
							if(PlayerID!= INDEX_NONE)
							{
								MobaCharacter->RegisterCharacterOnServer(PlayerID, CharacterID, ETeamType::ETT_None, ECharacterType::ECT_None);
							}
						}
					}
				}
			}
		}
	}
}

void AMobaGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(AMobaGameState * MobaGameState = GetGameState<AMobaGameState>())
	{
		if(AMobaPlayerState* MobaPlayerState = NewPlayer->GetPlayerState<AMobaPlayerState>())
		{
			if(UPlayerDataComponent* PlayerDataComponent = MobaPlayerState->GetPlayerDataComponent())
			{
				
				//TODO:拉取db服务器数据
				//赋值playerID

				//TODO:断线重连

				
				MobaGameState->MobaKillSystem.AddKiller(PlayerDataComponent->PlayerID);
			
			}
		}
	}
	
}
