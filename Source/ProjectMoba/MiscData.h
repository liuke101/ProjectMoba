// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiscData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerLocation
{
	GENERATED_BODY()

	FPlayerLocation(){};

	FPlayerLocation(const int64 InplayerID, const FVector& InLocation)
		: PlayerID(InplayerID),
		  Location(InLocation)
	{
	}

	UPROPERTY()
	int64 PlayerID = INDEX_NONE;

	UPROPERTY()
	FVector Location = FVector::ZeroVector;
	
};

/** Slot的属性 */
USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_BODY()

	FSlotData()
		: DataID(INDEX_NONE)
		, SlotIcon(nullptr)
		, CD(0.0f)
		, Number(INDEX_NONE)
		, MaxStackingQuantity(5)
		, bCancelBuy(true)
	{}

	UPROPERTY()
	int32 DataID;  //DataID用于读取DataTable, 相当于存储了该Slot的表

	UPROPERTY()
	TObjectPtr<UTexture2D> SlotIcon;

	UPROPERTY()
	float CD;

	UPROPERTY()
	int32 Number;
	
	UPROPERTY()
	int32 MaxStackingQuantity;

	UPROPERTY()
	bool bCancelBuy;

	void Reset()
	{
		DataID = INDEX_NONE;
		SlotIcon = nullptr;
		CD = 0.0f;
		Number = INDEX_NONE;
		MaxStackingQuantity = 5;
		bCancelBuy = true;
	}
};

USTRUCT(BlueprintType)
struct FSlotDataNetPackage
{
	GENERATED_BODY()

	FSlotDataNetPackage(){}
	
	UPROPERTY()
	TArray<int32> SlotIDs;

	UPROPERTY()
	TArray<FSlotData> SlotDatas; //存了DataID
};

/** 助攻 */
USTRUCT(BlueprintType)
struct FAssistPlayer
{
	GENERATED_BODY()

	FAssistPlayer(){}

	UPROPERTY()
	int64 PlayerID = INDEX_NONE;

	UPROPERTY()
	float AssistTime = 5.0f; //每次参与助攻就刷新时间
	
};

/** 击杀 */
USTRUCT(BlueprintType)
struct FKillSystem
{
	GENERATED_BODY()
public:
	//累计击杀
	struct FAccumulatedKill
	{
		FAccumulatedKill();
		FAccumulatedKill(const int64& InKillerPlayerID);
		
		int32 KillNumber; //一条命中的累计击杀
		int64 KillerPlayerID;
		int64 KilledPlayerID;
	};

	//连续击杀
	struct FContinuousKill
	{
		FContinuousKill();
		FContinuousKill(const int64& KillerPlayerID, const int64& KilledPlayerID);

		void ResetTime();
		
		int64 CurrentKillerPlayerID;
		int64 CurrentKilledPlayerID;
		int64 KillNumber; //KillTime内的连续击杀
		int64 KillTime;
	};

	// 记录
	TArray<FAccumulatedKill> AccumulatedKillHistory;
	TArray<FContinuousKill> ContinuousKillHistory;
	
	// 玩家登入时执行
	void AddKiller(const int64& InPlayerID);

	void Tick(float DeltaSeconds);

	void Kill(const int64& KillerPlayerID, const int64& KilledPlayerID);

	void Death(const int64& KilledPlayerID);

	//回调函数
	typedef TFunction<void(const int64&, const int64&)> TKillFunction;
	//普通击杀
	TKillFunction KillFunction;
	TKillFunction FirstBloodFuntion;

	//连续击杀
	TKillFunction DoubleKillFunction;
	TKillFunction TripleKillFunction;
	TKillFunction QuadraKillFunction;
	TKillFunction PentaKillFunction;

	//累计击杀
	TKillFunction DaShaTeShaFunction; //大杀特杀
	TKillFunction SuoXiangPiMiFunction; //所向披靡
	TKillFunction HunShenShiDanFunction; //浑身是胆
	TKillFunction YongGuanSanJunFunction; //勇冠三军
	TKillFunction YiJiDangQianFunction; //一骑当千
	TKillFunction WanFuMoDiFunction; //万夫莫敌
	TKillFunction JuShiWuShuangFunction; //举世无双
	TKillFunction TianXiaWuDiFunction; //天下无敌

	// 成就记录
	int64 FirstBlood = INDEX_NONE;
	TArray<int64> DoubleKillsArray;
	TArray<int64> TripleKillsArray;
	TArray<int64> QuadraKillsArray;
	TArray<int64> PentaKillsArray;

	TArray<int64> DaShaTeShaArray;
	TArray<int64> SuoXiangPiMiArray;
	TArray<int64> HunShenShiDanArray;
	TArray<int64> YongGuanSanJunArray;
	TArray<int64> YiJiDangQianArray;
	TArray<int64> WanFuMoDiArray;
	TArray<int64> JuShiWuShuangArray;
	TArray<int64> TianXiaWuDiArray;
	
};


FORCEINLINE bool operator==(const FAssistPlayer& Lhs, const FAssistPlayer& Rhs)
{
	return Lhs.PlayerID == Rhs.PlayerID;
}

FORCEINLINE bool operator==(const FKillSystem::FAccumulatedKill& Lhs, const FKillSystem::FAccumulatedKill& Rhs)
{
	return Lhs.KillerPlayerID == Rhs.KillerPlayerID;
}

FORCEINLINE bool operator==(const FKillSystem::FContinuousKill& Lhs, const FKillSystem::FContinuousKill& Rhs)
{
	return Lhs.CurrentKillerPlayerID == Rhs.CurrentKillerPlayerID;
}