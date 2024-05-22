// Fill out your copyright notice in the Description page of Project Settings.

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

	const TArray<FCharacterAsset*>* GetCharacterAssetsTemplate();
	const TArray<FCharacterAttribute*>* GetCharacterAttributesTemplate();
	FORCEINLINE const TMap<int64, FCharacterAttribute>* GetPlayerID_To_CharacterAttribute() const { return &PlayerID_To_CharacterAttribute; }

	void Add_PlayerID_To_CharacterAttribute(const int64 InPlayerID,const int32 InCharacterID); 
	
	const FCharacterAsset* GetCharacterAssetFromCharacterID(const int32 InCharacterID);
	const FCharacterAttribute* GetCharacterAttributeFromCharacterID(const int32 InCharacterID);
	const FCharacterAttribute* GetCharacterAttributeFromPlayerID(const int64 InPlayerID);

	void UpdateCharacterLocation(const int64 InPlayerID, const FVector& InLocation);
	void AddCharacterLocation(const int64 InPlayerID, const FVector& InLocation);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCharacterLocation(const int64 InPlayerID, FVector& OutLocation) const;

	FORCEINLINE const TArray<FPlayerLocation>& GetPlayerLocations() const { return PlayerLocations; }
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
    TObjectPtr<UDataTable> DT_CharacterAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TObjectPtr<UDataTable> DT_CharacterAttribute;
private:
	TArray<FCharacterAsset*> CacheCharacterAssets; 
	TArray<FCharacterAttribute*> CacheCharacterAttributes;
	TMap<int64, FCharacterAttribute> PlayerID_To_CharacterAttribute;  //PlayerID用于识别局内对象，不同于CharacterID，ChracterID用于识别不同英雄

	UPROPERTY(Replicated)
	TArray<FPlayerLocation> PlayerLocations;

	
};
