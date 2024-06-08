// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaAssistSystem.generated.h"

/** 助攻 */

struct FAssistPlayer;

USTRUCT(BlueprintType)
struct FMobaAssitSystem
{
	GENERATED_BODY()

public:
	FMobaAssitSystem();

	void Tick (float DeltaSeconds);
	
	FORCEINLINE TArray<FAssistPlayer> GetAssistPlayers() const { return AssistPlayers; }
	
	void AddAssistPlayer(const int64& InPlayerID);
	
	/** 获取最后一个助攻玩家 */
	const FAssistPlayer* GetLastAssistPlayer();

	void Death(const int64& KilledPlayerID);
	
private:
	/** 助攻列表（攻击我的敌方英雄） */
	TArray<FAssistPlayer> AssistPlayers;
};

USTRUCT(BlueprintType)
struct FAssistPlayer
{
	GENERATED_BODY()

	FAssistPlayer();

	UPROPERTY()
	int64 PlayerID;

	UPROPERTY()
	float AssistTime; //每次参与助攻就刷新时间

	FORCEINLINE void ResetAssitTime() { AssistTime = 10.0f; }
};

FORCEINLINE bool operator==(const FAssistPlayer& Lhs, const FAssistPlayer& Rhs)
{
	return Lhs.PlayerID == Rhs.PlayerID;
}

FORCEINLINE bool operator!=(const FAssistPlayer& Lhs, const FAssistPlayer& Rhs)
{
	return Lhs.PlayerID != Rhs.PlayerID;
}
