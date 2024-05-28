// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaPlayerState.h"

#include "ThreadManage.h"
#include "Component/PlayerDataComponent.h"
#include "ProjectMoba/MiscData.h"
#include "Table/SlotAsset.h"

AMobaPlayerState::AMobaPlayerState()
{
	PlayerDataComponent = CreateDefaultSubobject<UPlayerDataComponent>(TEXT("PlayerDataComponent"));
}

void AMobaPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// 创建6个Inventory Slot
	if(GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		for(int32 i = 0; i < 6; i++)
		{
			RecursionCreateInventorySlot();
		}
		
		// 延迟执行，等待客户端生成
		GThread::GetCoroutines().BindLambda(2.0f,[&]()
		{
			FSlotDataNetPackage NetPackage;
			GetInventorySlotNetPackage(NetPackage);
			Client_InitInventory(NetPackage);

			//测试(正式功能为点击购买)
			AddSlotToInventory(99990);
			AddSlotToInventory(99991);
		});
	}
}

const FSlotAsset* AMobaPlayerState::GetSlotAssetTemplate(const int32 InID)
{
	for(auto& SlotAsset : *GetSlotAssetsTemplate())
	{
		if(SlotAsset->ID == InID)
		{
			return SlotAsset;
		}
	}
	return nullptr;
}

const TArray<FSlotAsset*>* AMobaPlayerState::GetSlotAssetsTemplate()
{
	if(CacheSlotAssets.IsEmpty())
	{
		checkf(DT_SlotAsset, TEXT("SlotAssetTable为空，请在BP_MobaPlayerState中配置"));
		DT_SlotAsset->GetAllRows(TEXT("SlotAssetTable"), CacheSlotAssets);
	}

	return &CacheSlotAssets;
}

const FSlotAttribute* AMobaPlayerState::GetSlotAttributeTemplate(const int32 InID)
{
	for(auto& SlotAttribute : *GetSlotAttributesTemplate())
	{
		if(SlotAttribute->ID == InID)
		{
			return SlotAttribute;
		}
	}

	return nullptr;
}

const TArray<FSlotAttribute*>* AMobaPlayerState::GetSlotAttributesTemplate()
{
	if(CacheSlotAttributes.IsEmpty())
	{
		checkf(DT_SlotAttribute, TEXT("SlotAttributeTable为空，请在BP_MobaPlayerState中配置"));
		DT_SlotAttribute->GetAllRows(TEXT("SlotAttributeTable"), CacheSlotAttributes);
	}

	return &CacheSlotAttributes;
}

FSlotAttribute* AMobaPlayerState::GetSlotAttributeFromID(const int32 InID) const
{
	if(PlayerDataComponent->SlotAttribute_Internal.Contains(InID))
	{
		return PlayerDataComponent->SlotAttribute_Internal[InID];
	}
	return nullptr;
}

bool AMobaPlayerState::IsCDValid(int32 InID) const
{
	if(FSlotData* SlotData = GetInventorySlotData(InID))
	{
		return SlotData->CD <= 0.f;
	}
	// else if(0) //技能
	// {
	// 	
	// }
	return false;
}

bool AMobaPlayerState::AddSlotAttributes(const int32 InInventoryID, const int32 InSlotID)
{
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeTemplate(InSlotID))
	{
		//如果不为空，直接替换
		if(PlayerDataComponent->SlotAttribute_Internal.Contains(InInventoryID))
		{
			*PlayerDataComponent->SlotAttribute_Internal[InInventoryID] = *SlotAttribute;
		}
		else //否则直接添加
		{
			PlayerDataComponent->SlotAttribute_Internal.Add(InInventoryID, *SlotAttribute);
		}
		return true;
	}
	return false;
}

bool AMobaPlayerState::RecursionAddSlotAttributes(const int32 InSlotID)
{
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeTemplate(InSlotID))
	{
		int32 RandID = FMath::RandRange(0, 9999999);
		//如果为空，直接添加
		if(!PlayerDataComponent->SlotAttribute_Internal.Contains(RandID))
		{
			PlayerDataComponent->SlotAttribute_Internal.Add(RandID, *SlotAttribute);
			return true;
		}

		//否则
		return RecursionAddSlotAttributes(InSlotID);
	}
	return false;
}


void AMobaPlayerState::RecursionCreateInventorySlot()
{
	int32 InventoryID = FMath::RandRange(0, 9999999);
	if(!PlayerDataComponent->InventorySlots.Contains(InventoryID))
	{
		PlayerDataComponent->InventorySlots.Add(InventoryID, FSlotData());
		return;
	}

	return RecursionCreateInventorySlot();
}

bool AMobaPlayerState::AddSlotToInventory(int32 InSlotID)
{
	if(IsInventorySlotValid())
	{
		if(const FSlotAsset* SlotAsset = GetSlotAssetTemplate(InSlotID))
		{
			int32 InInventoryID = INDEX_NONE;
			for(auto& Tmp : *GetInventorySlots())
			{
				//找空位
				if(Tmp.Value.SlotID == INDEX_NONE)
				{
					Tmp.Value.Number = 3;
					Tmp.Value.SlotID = SlotAsset->ID; //SlotID对应DataTable, 用于获取对应的数据
					Tmp.Value.SlotIcon = SlotAsset->SlotIcon;
					InInventoryID = Tmp.Key; //InventoryID对应UI格子的ID
					break;
				}
			}

			if(InInventoryID != INDEX_NONE && AddSlotAttributes(InInventoryID, InSlotID)) //添加到Inventory
			{
				//通知本机客户端，更新Slot
				Client_UpdateSlot(InInventoryID, PlayerDataComponent->InventorySlots[InInventoryID]);
			}
			return true;
		}
	}

	return false;
}

bool AMobaPlayerState::IsInventorySlotValid() const
{
	// 如果有空位，返回true
	for(auto& Tmp : *GetInventorySlots())
	{
		if(Tmp.Value.SlotID == INDEX_NONE)
		{
			return true;
		}
	}
	return false;
}

bool AMobaPlayerState::IsInventorySlotValid(int32 InInventoryID)
{
	for(auto& Tmp : *GetInventorySlots())
	{
		if(Tmp.Key == InInventoryID)
		{
			return Tmp.Value.SlotID == INDEX_NONE;
		}
	}
	return false;
}

TMap<int32, FSlotData>* AMobaPlayerState::GetInventorySlots() const
{
	return &PlayerDataComponent->InventorySlots;
}


FSlotData* AMobaPlayerState::GetInventorySlotData(int32 InInventoryID) const
{
	if (GetInventorySlots()->Contains(InInventoryID))
	{
		return &GetInventorySlots()->FindChecked(InInventoryID);
	}

	return nullptr;
}

void AMobaPlayerState::GetAllInventoryIDs(TArray<int32>& InInventoryIDs)
{
	for(auto& Tmp : *GetInventorySlots())
	{
		InInventoryIDs.Add(Tmp.Key);
	}
}

void AMobaPlayerState::CheckInventory(int32 InInventoryID) const
{
	if(FSlotData* SlotData = GetInventorySlotData(InInventoryID))
	{
		//如果Slot上物品数量为0，重置、清空Slot
		if(SlotData->Number == 0)
		{
			SlotData->Reset();
			PlayerDataComponent->SlotAttribute_Internal.Remove(InInventoryID);
		}
	}
}


void AMobaPlayerState::GetInventorySlotNetPackage(FSlotDataNetPackage& OutNetPackage)
{
	for(auto& Tmp : *GetInventorySlots())
	{
		OutNetPackage.InventoryIDs.Add(Tmp.Key);
		OutNetPackage.SlotDatas.Add(Tmp.Value);
	}
}

void AMobaPlayerState::GetSkillSlotNetPackage(FSlotDataNetPackage& OutNetPackage)
{
	
}


void AMobaPlayerState::Server_UpdateInventory_Implementation(int32 InMoveInventoryID, int32 InTargetInventoryID)
{
	
}

void AMobaPlayerState::Server_Use_Implementation(int32 InInventoryID)
{
	
}

void AMobaPlayerState::Client_InitInventory_Implementation(const FSlotDataNetPackage& InSlotDataNetPackage)
{
	//清空旧数据
	PlayerDataComponent->InventorySlots.Empty(); 

	//读取网络包，加载到InventorySlots
	for(int32 i = 0; i < InSlotDataNetPackage.InventoryIDs.Num(); i++)
	{
		PlayerDataComponent->InventorySlots.Add(InSlotDataNetPackage.InventoryIDs[i], InSlotDataNetPackage.SlotDatas[i]);
	}

	//单播委托
	if(!InitSlotDelegate.ExecuteIfBound())
	{
		UE_LOG(LogTemp, Error, TEXT("InitSlotDelegate未绑定"));
	}
}

void AMobaPlayerState::Client_UpdateSlot_Implementation(int32 InInventoryID, const FSlotData& InNetSlotData)
{
	//更新Slot
	if(FSlotData* SlotData = GetInventorySlotData(InInventoryID))
	{
		
		*SlotData = InNetSlotData;
	}
	// else if(0) //技能
	// {
	// 	
	// }

	//单播委托
	if(!UpdateSlotDelegate.ExecuteIfBound(InInventoryID))
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateSlotDelegate未绑定"));
	}
}

void AMobaPlayerState::Client_StartUpdateCD_Implementation(int32 InInventoryID, const FSlotData& InNetSlotData)
{
	if(PlayerDataComponent->InventorySlots.Contains(InInventoryID))
	{
		PlayerDataComponent->InventorySlots[InInventoryID].CD = InNetSlotData.CD;
	}

	if(!StartUpdateCDSlotDelegate.ExecuteIfBound(InInventoryID))
	{
		UE_LOG(LogTemp, Error, TEXT("StartUpdateCDSlotDelegate未绑定"));
	}
	
}

void AMobaPlayerState::Client_EndUpdateCD_Implementation(int32 InInventoryID, const FSlotData& InNetSlotData)
{
	if(PlayerDataComponent->InventorySlots.Contains(InInventoryID))
	{
		PlayerDataComponent->InventorySlots[InInventoryID].CD = InNetSlotData.CD;
	}

	if(!EndUpdateCDSlotDelegate.ExecuteIfBound(InInventoryID))
	{
		UE_LOG(LogTemp, Error, TEXT("EndUpdateCDSlotDelegate未绑定"));
	}
}

