// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MobaAssistSystem.h"

#include "BehaviorTree/BehaviorTreeTypes.h"


FMobaAssitSystem::FMobaAssitSystem()
{
}

void FMobaAssitSystem::Tick(float DeltaSeconds)
{
	TArray<FAssistPlayer> RemoveAssistPlayers;
	for(auto& Tmp : AssistPlayers)
	{
		Tmp.AssistTime -= DeltaSeconds;
		//转换为每秒减一
			
		if(Tmp.AssistTime <= 0.f)
		{
			Tmp.AssistTime = 0.0f;
			RemoveAssistPlayers.Add(Tmp);
		}
	}
		
	for(auto& Tmp : RemoveAssistPlayers)
	{
		AssistPlayers.Remove(Tmp);
	}
}

void FMobaAssitSystem::AddAssistPlayer(const int64& InPlayerID)
{
	if(InPlayerID != INDEX_NONE)
	{
		FAssistPlayer AssistPlayer;
		AssistPlayer.PlayerID = InPlayerID;

		//如果已经存在助攻列表，更新时间
		if(AssistPlayers.Contains(AssistPlayer))
		{
			const int32 Index = AssistPlayers.Find(AssistPlayer);
			if(Index != INDEX_NONE)
			{
				AssistPlayers[Index].ResetAssitTime();
			}
		}
		else //不存在则添加
		{
			AssistPlayers.Add(AssistPlayer);
		}
	}
}

const FAssistPlayer* FMobaAssitSystem::GetLastAssistPlayer() 
{
	FAssistPlayer* LastAssitPlayer = nullptr;

	float LastTime = 0.0f;
	//遍历所有助攻玩家，找到最后一次助攻的玩家，即AssistTime最大的玩家
	for(auto& Tmp : AssistPlayers)
	{
		if(LastTime < Tmp.AssistTime)
		{
			LastTime = Tmp.AssistTime;
			LastAssitPlayer = &Tmp;
		}
	}

	return LastAssitPlayer;
}

void FMobaAssitSystem::Death()
{
	/** 清空助攻列表 */
	AssistPlayers.Empty();
}


FAssistPlayer::FAssistPlayer()
	: PlayerID(INDEX_NONE)
	, AssistTime(10.0f)
{
}
