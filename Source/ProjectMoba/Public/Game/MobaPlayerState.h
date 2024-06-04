#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProjectMoba/MiscData.h"
#include "MobaPlayerState.generated.h"

enum class ECharacterAttributeType : uint8;
struct FCharacterAttribute;
struct FSlotDataNetPackage;
struct FSlotData;
struct FSlotAttribute;
struct FSlotAsset;
class UDataTable;
class UPlayerDataComponent;


DECLARE_MULTICAST_DELEGATE_OneParam(FSimpleOneKeyDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FSimpleOneKeysDelegate, const TArray<int32>&);
DECLARE_DELEGATE_OneParam(FBindPlayerIDDelegate, int64);

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
	
	FBindPlayerIDDelegate BindPlayerIDDelegate; //绑定PlayerID
#pragma endregion

#pragma region DataTable数据读取 
public:
	const TArray<FSlotAsset*>* GetSlotAssets();
	const FSlotAsset* GetSlotAssetFromDataID(const int32 DataID);
	
	const TArray<FSlotAttribute*>* GetSlotAttributes();
	const FSlotAttribute* GetSlotAttributeFromDataID(const int32 DataID);
	FSlotAttribute* GetSlotAttributeFromSlotID(const int32 SlotID) const;
#pragma endregion

#pragma region Slot操作
public:
	/** Attribute 替换或添加到空Slot */
	bool AddSlotAttributes(int32 SlotID, int32 DataID);
	/** 递归添加到空Slot */
	bool RecursionAddSlotAttributes(int32 SlotID);
	
	FSlotData* GetSlotData(int32 SlotID) const;
	
	bool IsCDValid(int32 SlotID) const;

protected:
	/// 递归创建Slot
	/// @param CreateSlots 要创建的Slot
	/// @param CompareSlots 用于对比的Slot，防止ID重复
	void RecursionCreatelSlots(TMap<int32, FSlotData>& CreateSlots, TMap<int32, FSlotData>& CompareSlots);

	// 获取Slot对应的ID
	static void GetAllSlotIDs(const TMap<int32, FSlotData>& InSlots, TArray<int32>& OutSlotIDs);
#pragma  endregion

#pragma region Inventory库存
public:
	/** 递归创建InventorySlot */
	void RecursionCreateInventorySlots();
	/** 根据DataID获取DataTable数据，然后添加到InventorySlot */
	bool AddSlotToInventory(int32 DataID); 
	/** 检查InventorySlot是否有空位 */
	bool HasEmptyInventorySlot() const;
	/** 查询对应InventorySlot是否有效, 有则返回true */
	bool IsValidInventorySlot(int32 SlotID) const;
	/** 查询对应Item是否存在 */
	bool IsExistInInventory(int32 ItemDataID) const;
	
	FORCEINLINE TMap<int32, FSlotData>* GetInventorySlots() const;
	FSlotData* GetInventorySlotData(int32 SlotID) const;

	void GetAllInventorySlotIDs(TArray<int32>& OutSlotIDs) const;

	/** 检查物品数量，为0则清空Slot */
	void CheckInventory(int32 SlotID) const;

	/** 出售 */
	void Sell(int32 SlotID, int32 DataID, float Discount = 1.0f);
#pragma endregion
	
#pragma region 技能
public:
	/** 递归创建 SkillSlot */
	void RecursionCreateSkillSlots(); 
	FORCEINLINE TMap<int32, FSlotData>* GetSkillSlots() const;
	FSlotData* GetSkillSlotData(int32 SlotID) const;
	void InitSkillSlot();
	void GetAllSkillSlotIDs(TArray<int32>& OutSlotIDs) const;
	int32 GetSkillDataIDFromSlotID(int32 SlotID) const;
#pragma endregion

#pragma region 角色属性信息
	void UpdateCharacterInfo(const int64& InPlayerID);
#pragma endregion
	
#pragma region RPC
	//------------------数据-------------------
public:
	void GetInventorySlotNetPackage(FSlotDataNetPackage& OutNetPackage);
	void GetSkillSlotNetPackage(FSlotDataNetPackage& OutNetPackage);
private:
	void GetSlotNetPackage(TMap<int32, FSlotData>* InSlots, FSlotDataNetPackage& OutNetPackage);

	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerID(const int64 InPlayerID);

	//------------------Inventory-------------------//
public:
	// 交换和移动物品
	UFUNCTION(Server, Reliable)
	void Server_UpdateInventory(int32 MoveSlotID, int32 TargetSlotID);

	// 使用物品
	UFUNCTION(Server, Reliable)
	void Server_Use(int32 SlotID);

	// 购买物品
	UFUNCTION(Server, Reliable)
	void Server_Buy(int32 DataID);

	// 出售物品(打折)
	UFUNCTION(Server, Reliable)
	void Server_Sell(int32 SlotID, int32 DataID);

	// 取消购买
	UFUNCTION(Server, Reliable)
	void Server_CancelBuy(int32 SlotID, int32 DataID);

	UFUNCTION(Client, Reliable)
	void Client_InitInventorySlots(const FSlotDataNetPackage& SlotDataNetPackage);

	//------------------Skill-------------------//
	UFUNCTION(Client, Reliable)
	void Client_InitSkillSlots(const FSlotDataNetPackage& SlotDataNetPackage);

	//------------------通用-------------------//
	UFUNCTION(Client, Reliable)
	void Client_UpdateSlot(int32 SlotID, const FSlotData& NetSlotData);

	UFUNCTION(Client, Reliable)
	void Client_StartUpdateCD(int32 SlotID, const FSlotData& NetSlotData);

	UFUNCTION(Client, Reliable)
	void Client_EndUpdateCD(int32 SlotID, const FSlotData& NetSlotData);

	//------------------响应属性更新-------------------//
	//更新协议相应的属性
	UFUNCTION(Client, Reliable)
	void Client_ResponseUpdateCharacterAttribute(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType, float Value); 

	//更新整包
	UFUNCTION(Client, Reliable)
	void Client_ResponseUpdateAllCharacterAttributes(int64 InPlayerID, const FCharacterAttribute& CharacterAttribute);
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

	float GoldTime = 0.0f; //金币时间
	FVector HomeShopLocation = FVector::ZeroVector; //商店位置

#pragma endregion 
};
