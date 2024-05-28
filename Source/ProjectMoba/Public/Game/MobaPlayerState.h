#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProjectMoba/MiscData.h"
#include "MobaPlayerState.generated.h"

struct FSlotDataNetPackage;
struct FSlotData;
struct FSlotAttribute;
struct FSlotAsset;
class UDataTable;
class UPlayerDataComponent;


DECLARE_DELEGATE_OneParam(FSimpleOneKeyDelegate, int32);
DECLARE_DELEGATE_OneParam(FSimpleOneKeysDelegate, const TArray<int32>&);

UCLASS()
class PROJECTMOBA_API AMobaPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMobaPlayerState();

protected:
	virtual void BeginPlay() override;
	
public:
	FSimpleDelegate InitSlotDelegate;  //通知客户端
	FSimpleOneKeyDelegate UpdateSlotDelegate; //更新Slot
	FSimpleOneKeyDelegate StartUpdateCDSlotDelegate; //开始更新CD
	FSimpleOneKeyDelegate EndUpdateCDSlotDelegate; //结束更新CD
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
	
	bool AddSlotAttributes(int32 InInventoryID, int32 InSlotID); //直接添加到指定位置
	bool RecursionAddSlotAttributes(int32 InSlotID); //递归添加到空位置
	
	/** Inventory物品操作 */
	void RecursionCreateInventorySlot(); //递归创建InventorySlot
	bool AddSlotToInventory(int32 InSlotID); //根据SlotID获取DataTable数据，然后添加到InventorySlot
	
	bool IsInventorySlotValid() const; //检查InventorySlot是否有空位
	bool IsInventorySlotValid(int32 InInventoryID); //查询对应InventorySlot是否为空
	
	TMap<int32, FSlotData>* GetInventorySlots() const;
	FSlotData* GetInventorySlotData(int32 InInventoryID) const;
	

	void GetAllInventoryIDs(TArray<int32>& InInventoryIDs);
	
	void CheckInventory(int32 InInventoryID) const;

	

	
	/** 技能 */
	 

#pragma region RPC
public:
	//------------------数据-------------------
	void GetInventorySlotNetPackage(FSlotDataNetPackage& OutNetPackage);
	void GetSkillSlotNetPackage(FSlotDataNetPackage& OutNetPackage);

	//------------------Inventory-------------------//
	//交换和移动物品
	UFUNCTION(Server, Reliable)
	void Server_UpdateInventory(int32 InMoveInventoryID, int32 InTargetInventoryID);

	//使用物品
	UFUNCTION(Server, Reliable)
	void Server_Use(int32 InInventoryID);

	UFUNCTION(Client, Reliable)
	void Client_InitInventory(const FSlotDataNetPackage& InSlotDataNetPackage);

	UFUNCTION(Client, Reliable)
	void Client_UpdateSlot(int32 InInventoryID, const FSlotData& InNetSlotData);

	UFUNCTION(Client, Reliable)
	void Client_StartUpdateCD(int32 InInventoryID, const FSlotData& InNetSlotData);

	UFUNCTION(Client, Reliable)
	void Client_EndUpdateCD(int32 InInventoryID, const FSlotData& InNetSlotData);
	
protected:
	
#pragma endregion

	
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
