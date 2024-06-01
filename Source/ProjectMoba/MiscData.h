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

	FSlotData(){};

	UPROPERTY()
	int32 DataID = INDEX_NONE;  //DataID用于读取DataTable, 相当于存储了该Slot的表

	UPROPERTY()
	TObjectPtr<UTexture2D> SlotIcon= nullptr;

	UPROPERTY()
	float CD= 0.0f;

	UPROPERTY()
	int32 Number = INDEX_NONE;
	
	UPROPERTY()
	int32 MaxStackingQuantity = 5; 

	void Reset()
	{
		DataID = INDEX_NONE;
		SlotIcon = nullptr;
		CD = 0.0f;
		Number = INDEX_NONE;
	}
};

USTRUCT(BlueprintType)
struct FSlotDataNetPackage
{
	GENERATED_BODY()

	FSlotDataNetPackage(){};
	
	UPROPERTY()
	TArray<int32> SlotIDs;

	UPROPERTY()
	TArray<FSlotData> SlotDatas; //存了DataID
};