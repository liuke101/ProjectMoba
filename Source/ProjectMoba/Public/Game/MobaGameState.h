// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ProjectMoba/MobaType.h"
#include "Table/CharacterAttribute.h"
#include "MobaGameState.generated.h"

struct FPlayerLocation;
class UDataTable;
struct FCharacterAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateAllAttributesDelegate, int64 /*PlayerID*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdateAttributeDelegate, int64/*PlayerID*/, const ECharacterAttributeType);

UCLASS()
class PROJECTMOBA_API AMobaGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	FUpdateAllAttributesDelegate OnUpdateAllAttributesDelegate; //更新整包属性
	FUpdateAttributeDelegate OnUpdateAttributeDelegate; //更新协议指定属性
public:
	AMobaGameState();

protected:
	/** 复制 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	/** 从DataTable中读数据 */
	const TArray<FCharacterAsset*>* GetCachedCharacterAssets();
	const FCharacterAsset* GetCharacterAssetFromCharacterID(const int32 CharacterID); //CharacterID即DataID
	const FCharacterAsset* GetCharacterAssetFromPlayerID(const int64 PlayerID);
	const TArray<FCharacterAttribute*>* GetCachedCharacterAttributes();
	const FCharacterAttribute* GetCharacterAttributeFromCharacterID(const int32 CharacterID);
	

	/** CharacterAttribute */
	FORCEINLINE TMap<int64, FCharacterAttribute>* GetCharacterAttributes() {return &CharacterAttributes;}
	FCharacterAttribute* GetCharacterAttributeFromPlayerID(const int64 PlayerID);
	void AddCharacterAttribute(const int64 PlayerID,const int32 CharacterID); 

	/** ChracterLocation */
	void UpdateCharacterLocation(const int64 PlayerID, const FVector& Location);
	void AddCharacterLocation(const int64 PlayerID, const FVector& Location);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCharacterLocation(const int64 PlayerID, FVector& OutLocation) const;

	FORCEINLINE const TArray<FPlayerLocation>& GetPlayerLocations() const { return PlayerLocations; }

	/** ID */
	int32 GetCharacterIDFromPlayerID(const int64 PlayerID);

#pragma region RPC
public:
	/** 请求更新属性 */
	UFUNCTION(Server, Reliable)
	void Server_RequestUpdateCharacterAttribute(int64 PlayerID, const ECharacterAttributeType CharacterAttributeType);

protected:
	/** 响应更新属性 */
	//更新协议相应的属性
	UFUNCTION(Server, Reliable)
	void Server_ResponseUpdateCharacterAttribute(int64 PlayerID, const ECharacterAttributeType CharacterAttributeType, float Value); 

	//更新整包
	UFUNCTION(Server, Reliable)
	void Server_ResponseUpdateAllCharacterAttributes(int64 PlayerID, const FCharacterAttribute& CharacterAttribute); 
	
#pragma endregion
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Moba|DataTable")
    TObjectPtr<UDataTable> DT_CharacterAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|DataTable")
	TObjectPtr<UDataTable> DT_CharacterAttribute;
private:
	/** 存储DataTable数据 */
	TArray<FCharacterAsset*> CachedCharacterAssets; 
	TArray<FCharacterAttribute*> CachedCharacterAttributes;

	//在服务端，是角色数据
	//在客户端，是缓存池
	TMap<int64, FCharacterAttribute> CharacterAttributes;  //PlayerID用于识别局内对象，不同于CharacterID，ChracterID用于识别不同英雄

	UPROPERTY(Replicated)
	TArray<FPlayerLocation> PlayerLocations;
};
