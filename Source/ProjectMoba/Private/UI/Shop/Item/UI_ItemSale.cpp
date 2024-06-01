// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Item/UI_ItemSale.h"

#include "Blueprint/DragDropOperation.h"
#include "Common/MethodUnit.h"
#include "Components/TextBlock.h"
#include "UI/Inventory/UI_InventorySlot.h"
#include "UI/Shop/Item/UI_Item.h"

void UUI_ItemSale::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UUI_ItemSale::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool bDrop = false;

	if (UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		// 将背包物品拖拽到出售区域
		if (UUI_InventorySlot* MyInventorySlot = Cast<UUI_InventorySlot>(InDragDropOperation->Payload))
		{
			if(FSlotData* SlotData = GetOwningPlayerState<AMobaPlayerState>()->GetInventorySlotData(MyInventorySlot->GetSlotID()))
			{
				// 取消购买
				if(SlotData->bCancelBuy)
				{
					GetOwningPlayerState<AMobaPlayerState>()->Server_CancelBuy(MyInventorySlot->GetSlotID(), SlotData->DataID);
				}
				else // 直接出售
				{
					GetOwningPlayerState<AMobaPlayerState>()->Server_Sell(MyInventorySlot->GetSlotID(), SlotData->DataID);
				}
			}
			else
			{
				MyInventorySlot->UpdateSlot();
			}
			
			bDrop = true;
		}
	}

	return bDrop;
}

void UUI_ItemSale::SetSellText(const FText& InContent)
{
	SellText->SetText(InContent);
}

