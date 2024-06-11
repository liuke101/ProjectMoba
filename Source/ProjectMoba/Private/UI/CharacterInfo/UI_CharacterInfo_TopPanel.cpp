// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CharacterInfo/UI_CharacterInfo_TopPanel.h"

#include "ThreadManage.h"
#include "Components/UniformGridPanel.h"
#include "Game/MobaGameState.h"
#include "Game/MobaPlayerState.h"
#include "UI/Buff/UI_BuffBar.h"
#include "UI/CharacterInfo/UI_CharacterInfo.h"
#include "UI/Inventory/UI_SimpleInventorySlot.h"

void UUI_CharacterInfo_TopPanel::NativeConstruct()
{
	Super::NativeConstruct();

	//默认隐藏
	SetVisibility(ESlateVisibility::Hidden);
}

void UUI_CharacterInfo_TopPanel::BindDelegate()
{
	Super::BindDelegate();
	
	if(MobaGameState && MobaPlayerState)
	{
		MobaPlayerState->LookPlayerInfoDelegate.BindLambda([&](const FLookPlayerInfoNetPackage& InLookPlayerInfoNetPackage)
		{
			CharacterInfo->SetPlayerID(InLookPlayerInfoNetPackage.PlayerID);

			//清空
			CallAllSlot<UUI_SimpleInventorySlot>([&](UUI_SimpleInventorySlot* InSlot)
			{
				InSlot->ResetSlot();
				return true;
			});
		
			//更新Slot
			if(!InLookPlayerInfoNetPackage.SlotDataNetPackage.SlotIDs.IsEmpty() && !InLookPlayerInfoNetPackage.SlotDataNetPackage.SlotDatas.IsEmpty())
			{
				int32 i = 0;
				CallAllSlot<UUI_SimpleInventorySlot>([&](UUI_SimpleInventorySlot* InSlot)
				{
					InSlot->SetSlotID(InLookPlayerInfoNetPackage.SlotDataNetPackage.SlotIDs[i]);
					InSlot->UpdateSlot(InLookPlayerInfoNetPackage.SlotDataNetPackage.SlotDatas[i]);
					i++;
					return true;
				});
			}

			SetVisibility(ESlateVisibility::HitTestInvisible);
		});

		// 绑定隐藏TopPanel委托
		MobaPlayerState->HideTopPanelDelegate.BindLambda([&]()
		{
			SetVisibility(ESlateVisibility::Hidden);
			BuffBar-> Clear(); //清空Buff栏
		});

		// Buff信息委托
		MobaPlayerState->BuffInfoDelegate.BindLambda([&](const TArray<FBuffNetPackage>& InBuffNetPackages)
		{
			for(auto BuffNetPackage : InBuffNetPackages)
			{
				BuffBar->UpdateCD(BuffNetPackage.SlotID, BuffNetPackage.MaxCD);
			}
		});

		// 属性更新委托
		MobaGameState->OnUpdateAllAttributesDelegate.AddUObject(CharacterInfo, &UUI_CharacterInfo::ResponseUpdateSlots);
	}
	else
	{
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			BindDelegate();
		});
	}
}

UPanelWidget* UUI_CharacterInfo_TopPanel::GetSlotLayoutPanel()
{
	return SlotArrayInventory;
}
