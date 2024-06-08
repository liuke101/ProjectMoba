// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaType.h"
#include "MiscData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerLocation
{
	GENERATED_BODY()

	FPlayerLocation();
	FPlayerLocation(const int64 InplayerID, const FVector& InLocation);

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

	FSlotData();

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

	FSlotDataNetPackage();
	
	UPROPERTY()
	TArray<int32> SlotIDs;

	UPROPERTY()
	TArray<FSlotData> SlotDatas; //存了DataID
};

/** 击杀 */
USTRUCT(BlueprintType)
struct FKillNetPackgae
{
	GENERATED_BODY()

	FKillNetPackgae();

	UPROPERTY()
	EKillType KillType;

	UPROPERTY()
	TObjectPtr<UTexture2D> KillerIcon;
	
	UPROPERTY()
	FName KillerName;

	UPROPERTY()
	TObjectPtr<UTexture2D> KilledIcon;
	
	UPROPERTY()
	FName KilledName;
};

USTRUCT(BlueprintType)
struct FPlayerKDANetPackage
{
	GENERATED_BODY()

	FPlayerKDANetPackage();

	UPROPERTY()
	int32 KillNum; //击杀数

	UPROPERTY()
	int32 DeathNum; //死亡数

	UPROPERTY()
	int32 AssistNum; //助攻数

	UPROPERTY()
	int32 MinionKillNum; //补兵数
};

USTRUCT()
struct FPlayerInfoNetPackage
{
	GENERATED_BODY()

	FPlayerInfoNetPackage();
	
	UPROPERTY()
	int64 PlayerID;

	UPROPERTY()
	TObjectPtr<UTexture2D> PlayerIcon;

	UPROPERTY()
	int32 CharacterLevel;

	UPROPERTY()
	FPlayerKDANetPackage PlayerKDANetPackage;
};

USTRUCT(BlueprintType)
struct FPlayerTeamNetPackage
{
	GENERATED_BODY()

	UPROPERTY()
	FPlayerInfoNetPackage PlayerInfoNetPackage;

	UPROPERTY()
	FSlotDataNetPackage SlotDataNetPackage;
};
