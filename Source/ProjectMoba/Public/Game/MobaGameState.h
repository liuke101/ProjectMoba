﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Table/CharacterAttribute.h"
#include "MobaGameState.generated.h"

struct FPlayerLocation;
class UDataTable;
struct FCharacterAsset;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API AMobaGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMobaGameState();

protected:
	/** 复制 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	/** 从DataTable中读数据 */
	const TArray<FCharacterAsset*>* GetCharacterAssets();
	const FCharacterAsset* GetCharacterAssetFromCharacterID(const int32 CharacterID); //CharacterID即DataID
	const FCharacterAsset* GetCharacterAssetFromPlayerID(const int64 PlayerID);
	const TArray<FCharacterAttribute*>* GetCharacterAttributes();
	const FCharacterAttribute* GetCharacterAttributeFromCharacterID(const int32 CharacterID);
	FCharacterAttribute* GetCharacterAttributeFromPlayerID(const int64 PlayerID);

	/** CharacterAttribute */
	void AddCharacterAttribute(const int64 PlayerID,const int32 CharacterID); 

	/** ChracterLocation */
	void UpdateCharacterLocation(const int64 PlayerID, const FVector& Location);
	void AddCharacterLocation(const int64 PlayerID, const FVector& Location);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCharacterLocation(const int64 PlayerID, FVector& OutLocation) const;

	FORCEINLINE const TArray<FPlayerLocation>& GetPlayerLocations() const { return PlayerLocations; }

	/** ID */
	int32 GetCharacterIDFromPlayerID(const int64 PlayerID);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Moba|DataTable")
    TObjectPtr<UDataTable> DT_CharacterAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|DataTable")
	TObjectPtr<UDataTable> DT_CharacterAttribute;
private:
	/** 存储DataTable数据 */
	TArray<FCharacterAsset*> CacheCharacterAssets; 
	TArray<FCharacterAttribute*> CacheCharacterAttributes;
	
	TMap<int64, FCharacterAttribute> CharacterAttributes;  //PlayerID用于识别局内对象，不同于CharacterID，ChracterID用于识别不同英雄

	UPROPERTY(Replicated)
	TArray<FPlayerLocation> PlayerLocations;
};
