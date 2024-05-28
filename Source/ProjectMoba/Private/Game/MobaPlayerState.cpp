// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaPlayerState.h"
#include "Component/PlayerDataComponent.h"
#include "ProjectMoba/MiscData.h"
#include "Table/SlotAsset.h"

AMobaPlayerState::AMobaPlayerState()
{
	PlayerDataComponent = CreateDefaultSubobject<UPlayerDataComponent>(TEXT("PlayerDataComponent"));
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

bool AMobaPlayerState::AddSlotAttributes(const int32 InKey, const int32 InSlotID)
{
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeTemplate(InSlotID))
	{
		//如果不为空，直接替换
		if(PlayerDataComponent->SlotAttribute_Internal.Contains(InKey))
		{
			*PlayerDataComponent->SlotAttribute_Internal[InKey] = *SlotAttribute;
		}
		else //否则直接添加
		{
			PlayerDataComponent->SlotAttribute_Internal.Add(InKey, *SlotAttribute);
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

bool AMobaPlayerState::AddSlotToInventory(int32 InSlotID)
{
	if(IsInventorySlotValid())
	{
		if(const FSlotAsset* SlotAsset = GetSlotAssetTemplate(InSlotID))
		{
			int32 InInventoryID = INDEX_NONE;
			for(auto& Tmp : *GetInventorySlots())
			{
				if(Tmp.Value.SlotID == InSlotID)
				{
					Tmp.Value.Number = 3;
					Tmp.Value.SlotID = SlotAsset->ID;
					Tmp.Value.SlotIcon = SlotAsset->SlotIcon;
					InInventoryID = Tmp.Key;
					break;
				}
			}

			if(AddSlotAttributes(InInventoryID, InSlotID))
			{
				//通知本机客户端，更新Slot

				return true;
			}
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

void AMobaPlayerState::GetInventoryAllKeys(TArray<int32>& InKeys)
{
	for(auto& Tmp : *GetInventorySlots())
	{
		InKeys.Add(Tmp.Key);
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

void AMobaPlayerState::GetInventorySlotNetPackage(FSlotDataNetPackage& InNetPackage)
{
	for(auto& Tmp : *GetInventorySlots())
	{
		InNetPackage.SlotIDs.Add(Tmp.Key);
		InNetPackage.SlotDatas.Add(Tmp.Value);
	}
}

void AMobaPlayerState::GetSkillSlotNetPackage(FSlotDataNetPackage& InNetPackage)
{
	
}

