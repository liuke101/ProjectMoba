// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MobaGameState.generated.h"

struct FPlayerLocation;
class UDataTable;
struct FCharacterAttributeTable;
struct FCharacterAssetTable;
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

	const TArray<FCharacterAssetTable*>* GetCharacterAssetTables();
	const TArray<FCharacterAttributeTable*>* GetCharacterAttributeTables();
	const FCharacterAssetTable* GetCharacterAssetTable(const int32& InID);
	const FCharacterAttributeTable* GetCharacterAttributeTable(const int32& InID);

	void UpdateCharacterLocation(const int32 InID, const FVector& InLocation);
	void AddCharacterLocation(const int32 InID, const FVector& InLocation);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCharacterLocation(const int32 InID, FVector& OutLocation) const;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
    TObjectPtr<UDataTable> CharacterAssetTablePtr;

	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	TObjectPtr<UDataTable> CharacterAttributeTablePtr;
private:
	TArray<FCharacterAssetTable*> CacheCharacterAssetTables; //因为FCharacterTable有反射， 所以不能使用前置声明，直接include????对吗??
	TArray<FCharacterAttributeTable*> CacheCharacterAttributeTables;

	UPROPERTY(Replicated)
	TArray<FPlayerLocation> PlayerLocations; 
};
