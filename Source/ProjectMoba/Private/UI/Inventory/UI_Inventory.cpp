// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/UI_Inventory.h"

#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/StaticMeshComponent.h"
#include "UI/Inventory/UI_InventorySlot.h"

void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	LayoutInventroySlot(3, 2); //初始化3*2的格子
}

void UUI_Inventory::LayoutInventroySlot(int32 ColumNumber, int32 RowNumber) const
{
	if (InventorySlotClass)
	{
		for (int32 MyRow = 0; MyRow < RowNumber; MyRow++)
		{
			for (int32 MyColum = 0; MyColum < ColumNumber; MyColum++)
			{
				if (UUI_InventorySlot* SlotWidget = CreateWidget<UUI_InventorySlot>(GetWorld(), InventorySlotClass))
				{
					if (UUniformGridSlot* GridSlot = SlotArrayInventory->AddChildToUniformGrid(SlotWidget))
					{
						GridSlot->SetColumn(MyColum);
						GridSlot->SetRow(MyRow);
						GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
						GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
					}
					SlotWidget->SetSlotID(MyRow * ColumNumber + MyColum);
					//Update
					SlotWidget->UpdateSlot();
				}
			}
		}
	}
}

void UUI_Inventory::CallInventorySlotBreak(TFunction<bool(UUI_InventorySlot*)> InventorySlotLamada) const
{
	for (const auto& TmpSlot : SlotArrayInventory->GetAllChildren())
	{
		if (UUI_InventorySlot* InventorySlot = Cast<UUI_InventorySlot>(TmpSlot))
		{
			if (!InventorySlotLamada(InventorySlot))
			{
				break;
			}
		}
	}
}
