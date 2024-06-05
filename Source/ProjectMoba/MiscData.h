// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaType.h"
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

FORCEINLINE bool operator==(const FAssistPlayer& Lhs, const FAssistPlayer& Rhs)
{
	return Lhs.PlayerID == Rhs.PlayerID;
}

FORCEINLINE bool operator!=(const FAssistPlayer& Lhs, const FAssistPlayer& Rhs)
{
	return Lhs.PlayerID != Rhs.PlayerID;
}

USTRUCT(BlueprintType)
struct FKillNetPackgae
{
	GENERATED_BODY()

	FKillNetPackgae()
		: KillType(EKillType::EKT_NormalKill)
		, KillerName("DefaultKiller")
		, KilledName("DefaultKilled")
	{}

	UPROPERTY()
	EKillType KillType;

	UPROPERTY()
	TObjectPtr<UTexture2D> KillerIcon;
	
	UPROPERTY()
	FString KillerName;

	UPROPERTY()
	TObjectPtr<UTexture2D> KilledIcon;
	
	UPROPERTY()
	FString KilledName;
};