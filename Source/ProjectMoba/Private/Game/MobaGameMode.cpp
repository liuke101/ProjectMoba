// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameMode.h"

#include "ThreadManage.h"
#include "Character/MobaCharacter.h"
#include "Character/Hero/MobaHeroCharacter.h"
#include "Character/Tool/CharacterSpawnPoint.h"
#include "Common/MethodUnit.h"
#include "Component/MobaKillSystemComponent.h"
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
					continue;
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
								MobaCharacter->RegisterCharacterOnServer(PlayerID, CharacterID, CharacterSpawnPoint->GetTeamType() , CharacterSpawnPoint->GetCharacterType());
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
				//随机playerID
				MobaPlayerState->GetPlayerDataComponent()->PlayerID = FMath::RandRange(0, 1000000);
				
				MobaPlayerState->Client_UpdatePlayerData(MobaPlayerState->GetPlayerDataComponent()->PlayerID);
				
				MobaGameState->KillSystemComponent->AddKiller(PlayerDataComponent->PlayerID);

				//TODO:断线重连、
				//遍历场景内有没有和我们的ID一样的角色，如果有就控制它

				//玩家生成
				// 从txt读取角色ID，然后根据角色ID生成角色
				
				GThread::GetCoroutines().BindLambda(0.5f, [](AMobaGameState * InGameState,AMobaPlayerState* InPlayerState)
				{
					if(AMobaPawn* MobaPawn = InPlayerState->GetPawn<AMobaPawn>())
					{
						TArray<FString> NumberStrings;
						FFileHelper::LoadFileToStringArray(NumberStrings, *(FPaths::ProjectDir() / TEXT("CharacterID.txt"))); 
						int32 CharacterID = FCString::Atoi(*NumberStrings[0]);

						//InPlayerState->GetPlayerDataComponent()->TeamType = static_cast<ETeamType>(FCString::Atoi(*NumberStrings[1]));
						//随机队伍
						InPlayerState->GetPlayerDataComponent()->TeamType = FMath::RandBool() ? ETeamType::ETT_Red : ETeamType::ETT_Blue;

						//生成英雄
						if(AMobaHeroCharacter* Hero = Cast<AMobaHeroCharacter>(InGameState->SpawnPlayerComponent->SpawnCharater(InPlayerState->GetPlayerID(), CharacterID, InPlayerState->GetPlayerDataComponent()->TeamType)))
						{
							//控制该英雄
							MobaPawn->SetControlledMobaHero(Hero);

							//镜头变换到英雄位置
							MobaPawn->SetActorLocation(Hero->GetActorLocation());
						}
					}
				}, MobaGameState, MobaPlayerState);
			}
		}
	}
}
