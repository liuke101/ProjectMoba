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


DECLARE_MULTICAST_DELEGATE_OneParam(FSimpleOneKeyDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FSimpleOneKeysDelegate, const TArray<int32>&);

UCLASS()
class PROJECTMOBA_API AMobaPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMobaPlayerState();

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
	
public:
#pragma region Delegate
	FSimpleMulticastDelegate InitSlotDelegate;  //通知客户端
	FSimpleOneKeyDelegate UpdateSlotDelegate; //更新Slot
	FSimpleOneKeyDelegate StartUpdateCDSlotDelegate; //开始更新CD
	FSimpleOneKeyDelegate EndUpdateCDSlotDelegate; //结束更新CD
#pragma endregion

	
public:
	/** 从DataTable中读数据 */
	const FSlotAsset* GetSlotAssetTemplate(const int32 InID);
	const TArray<FSlotAsset*>* GetSlotAssetsTemplate();
	const FSlotAttribute* GetSlotAttributeTemplate(const int32 InID);
	const TArray<FSlotAttribute*>* GetSlotAttributesTemplate();


	/** 对Slot实例的操作 */
	FSlotAttribute* GetSlotAttributeFromID(const int32 InInventoryID) const;
	bool IsCDValid(int32 InInventoryID) const;

	/** 数据属性操作 */
	bool AddSlotAttributes(int32 InInventoryID, int32 InSlotID); //直接添加到指定位置
	bool RecursionAddSlotAttributes(int32 InSlotID); //递归添加到空位置
	FSlotData* GetSlotData(int32 ID) const; //根据ID获取SlotData, 可能是InventorySlot或者SkillSlot

	/** Inventory物品操作 */
	void RecursionCreateInventorySlots(); //递归创建InventorySlot
	bool AddSlotToInventory(int32 InSlotID); //根据SlotID获取DataTable数据，然后添加到InventorySlot
	
	bool HasEmptyInventorySlot() const; //检查InventorySlot是否有空位
	bool IsValidInventorySlot(int32 InInventoryID); //查询对应InventorySlot是否有物品, 有则返回true
	
	TMap<int32, FSlotData>* GetInventorySlots() const;
	FSlotData* GetInventorySlotData(int32 InInventoryID) const;

	void GetAllInventoryIDs(TArray<int32>& OutInventoryIDs) const;

	/** 检查物品数量，为0则清空Slot */
	void CheckInventory(int32 InInventoryID) const;
	
	/** 技能 */
	void RecursionCreateSkillSlots(); //递归创建SkillSlot
	TMap<int32, FSlotData>* GetSkillSlots() const;
	FSlotData* GetSkillSlotData(int32 InSkillID) const;
	void InitSkillSlot();
	void GetAllSkillIDs(TArray<int32>& OutSkillIDs) const;
	

protected:
	/// 递归创建Slot
	/// @param CreateSlots 要创建的Slot
	/// @param CompareSlots 用于对比的Slot，防止ID重复
	void RecursionCreatelSlots(TMap<int32, FSlotData>& CreateSlots, TMap<int32, FSlotData>& CompareSlots);

	// 获取Slot对应的ID
	static void GetAllIDs(const TMap<int32, FSlotData>& InSlots, TArray<int32>& OutIDs);
	
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
	void Client_InitInventorySlots(const FSlotDataNetPackage& InSlotDataNetPackage);

	//------------------Skill-------------------//
	UFUNCTION(Client, Reliable)
	void Client_InitSkillSlots(const FSlotDataNetPackage& InSlotDataNetPackage);

	//------------------通用-------------------//
	UFUNCTION(Client, Reliable)
	void Client_UpdateSlot(int32 InInventoryID, const FSlotData& InNetSlotData);

	UFUNCTION(Client, Reliable)
	void Client_StartUpdateCD(int32 InInventoryID, const FSlotData& InNetSlotData);

	UFUNCTION(Client, Reliable)
	void Client_EndUpdateCD(int32 InInventoryID, const FSlotData& InNetSlotData);
#pragma endregion


#pragma region 成员变量
public:
	FORCEINLINE UPlayerDataComponent* GetPlayerDataComponent() const { return PlayerDataComponent; }
	
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
#pragma endregion 
};
