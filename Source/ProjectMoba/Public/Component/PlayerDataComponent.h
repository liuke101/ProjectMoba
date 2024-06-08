// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectMoba/MiscData.h"
#include "ProjectMoba/MobaType.h"
#include "Table/SlotAttribute.h"
#include "PlayerDataComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UPlayerDataComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AMobaPlayerState;
	
public:
	UPlayerDataComponent();

protected:
	virtual void BeginPlay() override;
	
	/** 复制 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	FName PlayerName;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	ETeamType TeamType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	int64 PlayerID; 

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	int32 Gold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	int32 KillNum; //击杀数

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	int32 DeathNum; //死亡数

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	int32 AssistNum; //助攻数

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	int32 MinionKillNum; //补兵数
	
	// 服务端物品栏x6
	// 通过RPC状态同步到客户端,TMap不支持RPC，我们使用TArray来将其拆分, 为此我们创建了一个结构体FSlotDataNetPackage
	TMap<int32, FSlotData> InventorySlots; //SlotID_To_SlotData

	// 服务端技能栏x4
	TMap<int32, FSlotData> SkillSlots; //SlotID_To_SlotData

private:
	struct FSlotAttribute_Internal
	{
		//模拟Map
		struct FSlotAttribute_Element
		{
			FSlotAttribute_Element()
				: Key(INDEX_NONE)
			{
			}
			
			//Key: SlotID 表示格子的ID
			//Value: FSlotAttribute
			FSlotAttribute_Element(int32 InKey, const FSlotAttribute& InValue)
				: Key(InKey), Value(InValue)
			{
			}
			
			int32 Key;
			FSlotAttribute Value;

			bool IsValid() const { return Key != INDEX_NONE; }
			
			bool operator==(const FSlotAttribute_Element& InElement) const
			{
				return Key == InElement.Key;
			}
		};

		
		bool Contains(int32 InKey);
		void Add(int32 Key, const FSlotAttribute& Value);
		void Remove(int32 InKey);

		void SetKeyToNewKey(int32 OldKey, int32 NewKey); //移动
		void SwapKey(int32 KeyA, int32 KeyB); //交换

		FSlotAttribute* operator[](int32 InID)
		{
			for(auto& Tmp : AttributeElements)
			{
				if(Tmp.Key == InID)
				{
					return &Tmp.Value;
				}
			}
			return nullptr;
		}


		TArray<FSlotAttribute_Element> AttributeElements;
		
	}SlotAttribute_Internal;

	// CD队列，专门计算技能、物品、装备、属性
	// 推送到队列中，在队列中计算cd，然后再推送到客户端
	TMap<int32, FSlotData*> SlotCDQueue; //Slot_To_SlotData

	// 距离队列，专门计算售卖
	TMap<int32, FSlotData*> SaleSlotDistanceQueue; //Slot_To_SlotData
};
