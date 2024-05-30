// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/UI_InventoryBar.h"

#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Game/MobaPlayerState.h"
#include "UI/Inventory/UI_InventorySlot.h"


void UUI_InventoryBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_InventoryBar::InitSlotLayout() const
{
	if (InventorySlotClass)
	{
		if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
		{
			//获取所有InventoryID
			TArray<int32> InventoryIDs;
			MobaPlayerState->GetAllInventoryIDs(InventoryIDs);
			
			for (int32 x = 0; x < Layout_Row; x++)
			{
				for (int32 y = 0; y < Layout_Col; y++)
				{
					if (UUI_InventorySlot* SlotWidget = CreateWidget<UUI_InventorySlot>(GetWorld(), InventorySlotClass))
					{
						if (UUniformGridSlot* GridSlot = InventorySlotArray->AddChildToUniformGrid(SlotWidget))
						{
							GridSlot->SetColumn(y);
							GridSlot->SetRow(x);
							GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
							GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
						}
						//设置InventorySlotID(即InventoryID)
						SlotWidget->SetSlotID(InventoryIDs[x * Layout_Col + y]);
						
						//更新Slot
						SlotWidget->UpdateSlot();
					}
				}
			}
		}
	}
}

UPanelWidget* UUI_InventoryBar::GetSlotLayoutPanel() const
{
	return InventorySlotArray;
}
