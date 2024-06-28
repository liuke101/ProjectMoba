﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Spawn/MobaSpawnPlayerComponent.h"
#include "GameFramework/GameStateBase.h"
#include "ProjectMoba/MobaType.h"
#include "Table/CharacterAttribute.h"
#include "MobaGameState.generated.h"

class UMobaSpawnMonsterComponent;
class AMobaCharacter;
class AMobaPlayerState;
class UMobaKillSystemComponent;
class UMobaSpawnMinionComponent;
class UMobaMinionSystem;
struct FMobaAssitSystem;
struct FAssistPlayer;
struct FKillNetPackgae;
struct FMobaKillSystem;
struct FPlayerLocation;
class UDataTable;
struct FCharacterAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateAllAttributesDelegate, int64 /*PlayerID*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdateAttributeDelegate, int64/*PlayerID*/, const ECharacterAttributeType);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FUpdateBuffBarDelegate, int64 /*PlayerID*/,int32/*DataID*/, float/*CD*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FEndBuffBarDelegate, int64 /*PlayerID*/,int32/*DataID*/);

UCLASS()
class PROJECTMOBA_API AMobaGameState : public AGameStateBase
{
	GENERATED_BODY()

	friend class AMobaGameMode;
	
public:
	FUpdateAllAttributesDelegate OnUpdateAllAttributesDelegate; //更新整包属性
	FUpdateAttributeDelegate OnUpdateAttributeDelegate; //更新协议指定属性
	FUpdateBuffBarDelegate UpdateBuffDelegate; //更新Buff
	FEndBuffBarDelegate EndBuffDelegate; //结束Buff
public:
	AMobaGameState();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/** 复制 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** 从DataTable中读数据 */
	const TArray<FCharacterAsset*>* GetCachedCharacterAssets();
	const FCharacterAsset* GetCharacterAssetFromCharacterID(const int32 CharacterID); //CharacterID即DataID
	const FCharacterAsset* GetCharacterAssetFromPlayerID(const int64 PlayerID);
	const TArray<FCharacterAttribute*>* GetCachedCharacterAttributes();
	const FCharacterAttribute* GetCharacterAttributeFromCharacterID(const int32 CharacterID);
	
	int32 GetSkillDataIDFroSkillMontage(const UAnimMontage* AnimMontage);
	const FCharacterAsset* GetCharacterAssetFromSkillDataID(const int32 SkillDataID); 
	
	/** CharacterAttribute */
	FORCEINLINE TMap<int64, FCharacterAttribute>* GetCharacterAttributes() {return &CharacterAttributes;}
	FCharacterAttribute* GetCharacterAttributeFromPlayerID(const int64 PlayerID);
	void AddCharacterAttribute(const int64 PlayerID,const int32 CharacterID); 

	/** ChracterLocation */
	void UpdateCharacterLocation(const int64 PlayerID, const FVector& Location);
	void AddCharacterLocation(const int64 PlayerID, const FVector& Location);
	void RemoveCharacterLocation(const int64 PlayerID);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCharacterLocation(const int64 PlayerID, FVector& OutLocation) const;

	FORCEINLINE const TArray<FPlayerLocation>& GetPlayerLocations() const { return PlayerLocations; }

	/** ID */
	int32 GetCharacterIDFromPlayerID(const int64 PlayerID);

#pragma region RPC
	//GameState不能执行ClentRPC，我们通过在PlayerState中执行ClientRPC来调用下列接口
public:
	/** 请求更新属性 */
	void RequestUpdateCharacterAttribute(int64 PlayerID, int64 UpdatedPlayerID, const ECharacterAttributeType CharacterAttributeType);

	/** 响应更新属性 */
	//更新协议相应的属性
	void ResponseUpdateCharacterAttribute(int64 PlayerID, const ECharacterAttributeType CharacterAttributeType, float Value); 
	//更新整包
	void ResponseUpdateAllCharacterAttributes(int64 PlayerID, const FCharacterAttribute& CharacterAttribute);

	/** 更新击杀信息 */
	void UpdateKillMessage(const FKillNetPackgae& KillNetPackgae) const;

	/** 客户端更新buff */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateBuff(int64 InPlayerID, int32 DataID, float CD);

	/** 客户端结束buff */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_EndBuff(int64 InPlayerID, int32 DataID);

	/** 广播角色属性变化 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_CharacterAttributeChanged(AMobaCharacter* InCharacter, const ECharacterAttributeType CharacterAttributeType, float Value);
	
#pragma endregion
	
	
#pragma region 战斗
	/** 死亡结算 */
	void SettleDeath(int64 KillerPlayerID, int64 KilledPlayerID);
	bool IsPlayer(int64 PlayerID) const;
	void Death(int64 PlayerID);
	/** 击杀系统 */
	void BindKillFuntion();
	void MulticastKillMessage(EKillType KillType, int64 KillerPlayerID, int64 KilledPlayerID);

	/** 团队击杀数 */
	void AddTeamKillCount(const ETeamType TeamType, const int32 KillCount);

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

	/** 击杀系统 */
	UPROPERTY(EditDefaultsOnly, Category = "Moba|Component")
	TObjectPtr<UMobaKillSystemComponent> KillSystemComponent;
	
	/** 团队杀敌数 */
	TMap<ETeamType, int32> TeamKillCount;

	/** 小兵生成 */
	UPROPERTY(EditDefaultsOnly, Category = "Moba|Component")
	TObjectPtr<UMobaSpawnMinionComponent> SpawnMinionComponent;

	/** 怪物生成 */
	UPROPERTY(EditDefaultsOnly, Category = "Moba|Component")
	TObjectPtr<UMobaSpawnMonsterComponent> SpawnMonsterComponent;

	/** 玩家生成 */
	UPROPERTY(EditDefaultsOnly, Category = "Moba|Component")
	TObjectPtr<UMobaSpawnPlayerComponent> SpawnPlayerComponent;
};
