#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MobaPlayerState.generated.h"

struct FSlotDataNetPackage;
struct FSlotData;
struct FSlotAttribute;
struct FSlotAsset;
class UDataTable;
class UPlayerDataComponent;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API AMobaPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMobaPlayerState();

public:
	FORCEINLINE UPlayerDataComponent* GetPlayerDataComponent() const { return PlayerDataComponent; }

	/** 从DataTable中读数据 */
	const FSlotAsset* GetSlotAssetTemplate(const int32 InID);
	const TArray<FSlotAsset*>* GetSlotAssetsTemplate();
	const FSlotAttribute* GetSlotAttributeTemplate(const int32 InID);
	const TArray<FSlotAttribute*>* GetSlotAttributesTemplate();


	/** 对Slot实例的操作 */
	FSlotAttribute* GetSlotAttributeFromID(const int32 InID) const;
	bool IsCDValid(int32 InID) const;
	
	bool AddSlotAttributes(int32 InKey, int32 InSlotID); //直接添加到指定位置
	bool RecursionAddSlotAttributes(int32 InSlotID); //递归添加到空位置
	
	/** Inventory物品操作 */
	bool AddSlotToInventory(int32 InSlotID);
	
	bool IsInventorySlotValid() const; //检查Inventory是否有空位
	bool IsInventorySlotValid(int32 InInventoryID);
	
	TMap<int32, FSlotData>* GetInventorySlots() const;
	FSlotData* GetInventorySlotData(int32 InInventoryID) const;

	void GetInventoryAllKeys(TArray<int32>& InKeys);
	
	void CheckInventory(int32 InInventoryID) const;

	/** 技能 */
	 

	/** RPC */
	void GetInventorySlotNetPackage(FSlotDataNetPackage& InNetPackage);
	void GetSkillSlotNetPackage(FSlotDataNetPackage& InNetPackage);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|Component")
	TObjectPtr<UPlayerDataComponent> PlayerDataComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|DataTable")
	TObjectPtr<UDataTable> DT_SlotAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|DataTable")
	TObjectPtr<UDataTable> DT_SlotAttribute;

private:
	/** 存储DataTable数据 */
	TArray<FSlotAsset*> CacheSlotAssets; 
	TArray<FSlotAttribute*> CacheSlotAttributes;
};
