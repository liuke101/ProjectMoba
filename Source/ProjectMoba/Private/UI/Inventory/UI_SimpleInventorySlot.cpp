// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/UI_SimpleInventorySlot.h"

#include "ProjectMoba/MiscData.h"

void UUI_SimpleInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	
}

void UUI_SimpleInventorySlot::UpdateSlot(const FSlotData* SlotData)
{
	//SlotData是服务器传来的数据包
	if(SlotData)
	{
		ClientCacheSlotData = *SlotData; //缓存数据包
		
		UpdateIcon(SlotData->SlotIcon);
		SetTextNumber(SlotNumber, SlotData->Number);
	}
}

void UUI_SimpleInventorySlot::ResetSlot()
{
	Super::ResetSlot();
	SetTextNumber(SlotNumber, INDEX_NONE);
	SetSlotID(INDEX_NONE);
}

UUI_Tip* UUI_SimpleInventorySlot::GetTip()
{
	if(MobaPlayerState)
	{
		if(VerifyTip(MobaPlayerState, Super::GetTip(), GetSlotID(), &ClientCacheSlotData))
		{
			return Tip;
		}
	}
	return nullptr;
}
