// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Info/UI_CharacterInfo_TopPanel.h"

#include "ThreadManage.h"
#include "Components/UniformGridPanel.h"
#include "Game/MobaGameState.h"
#include "Game/MobaPlayerState.h"
#include "UI/Info/UI_CharacterInfo.h"
#include "UI/Inventory/UI_InventorySlot.h"

void UUI_CharacterInfo_TopPanel::NativeConstruct()
{
	Super::NativeConstruct();

	//绑定委托
	GThread::GetCoroutines().BindLambda(0.3f, [&]()
	{
		
		if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
		{
			//ID绑定委托
			MobaPlayerState->BindPlayerIDDelegate.BindLambda([&](int64 InPlayerID)
			{
				CharacterInfo->SetPlayerID(InPlayerID);

				// 清空Slot
				CallAllSlot<UUI_Slot>([&](UUI_Slot* InSlot)
				{
					InSlot->SetSlotID(INDEX_NONE);
					InSlot->UpdateSlot();
					return true;
				});
			});

			// 绑定初始化Slot分布
			MobaPlayerState->InitSlotDelegate.AddLambda([&](const TArray<int32>& InSlotIDs)
			{
				InitSlotLayout(InSlotIDs);  
			});
		}
		
		// 属性更新委托
		if(AMobaGameState* MobaGameState = GetMobaGameState())
		{
			MobaGameState->OnUpdateAllAttributesDelegate.AddUObject(CharacterInfo, &UUI_CharacterInfo::ResponseUpdateSlots);
		}
	});
}

void UUI_CharacterInfo_TopPanel::InitSlotLayout(const TArray<int32>& SlotIDs)
{
	if(SlotIDs.IsEmpty()) return;
	
	TArray<int32> Tmp = SlotIDs;
	CallAllSlot<UUI_Slot>([&](UUI_Slot* InSlot)
	{
		if(!Tmp.IsEmpty())
		{
			InSlot->SetSlotID(Tmp.Pop());
			InSlot->UpdateSlot();
		}
		return true;
	});
}

UPanelWidget* UUI_CharacterInfo_TopPanel::GetSlotLayoutPanel()
{
	return SlotArrayInventory;
}
