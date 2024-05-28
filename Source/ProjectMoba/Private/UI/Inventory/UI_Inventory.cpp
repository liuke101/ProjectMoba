// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/UI_Inventory.h"

#include "ThreadManage.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/StaticMeshComponent.h"
#include "Game/MobaPlayerState.h"
#include "UI/Inventory/UI_InventorySlot.h"


void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	GThread::GetCoroutines().BindUObject(0.5f, this, &UUI_Inventory::BindSlotDelegate);
}

void UUI_Inventory::InitInventroySlotLayout(const int32 ColumNumber, const int32 RowNumber) const
{
	if (InventorySlotClass)
	{
		if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
		{
			//获取所有InventoryID
			TArray<int32> InventoryIDs;
			MobaPlayerState->GetAllInventoryIDs(InventoryIDs);
			
			
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
						//设置InventorySlotID(即InventoryID)
						SlotWidget->SetSlotID(InventoryIDs[MyRow * ColumNumber + MyColum]);
						
						//更新Slot
						SlotWidget->UpdateSlot();
					}
				}
			}
		}
	}
}


void UUI_Inventory::BindSlotDelegate()
{
	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		// 绑定初始化Slot分布
		MobaPlayerState->InitSlotDelegate.BindLambda([&]()
		{
			InitInventroySlotLayout(3, 2);  
		});

		// 绑定更新ID对应的Slot
		MobaPlayerState->UpdateSlotDelegate.BindLambda([&](int32 InventoryID)
		{
			CallAllInventorySlot([&](UUI_InventorySlot* InventorySlot)
			{
				if(InventorySlot->GetSlotID() == InventoryID)
				{
					InventorySlot->UpdateSlot();
					return false; //如果找到停止Call
				}
				return true;
			});
		});

		// 绑定开始更新CD
		MobaPlayerState->StartUpdateCDSlotDelegate.BindLambda([&](const int32 SlotID)
		{
			CallAllInventorySlot([&](UUI_InventorySlot* InventorySlot)
			{
				if (InventorySlot->GetSlotID() == SlotID)
				{
					InventorySlot->StartUpdateCD();
					return false;
				}
				return true;
			});
		});

		// 绑定停止更新CD
		MobaPlayerState->EndUpdateCDSlotDelegate.BindLambda([&](const int32 SlotID)
		{
			CallAllInventorySlot([&](UUI_InventorySlot* InventorySlot)
			{
				if (InventorySlot->GetSlotID() == SlotID)
				{
					InventorySlot->EndUpdateCD();
					return false;
				}
				return true;
			});
		});
	}
}

void UUI_Inventory::CallAllInventorySlot(TFunction<bool(UUI_InventorySlot*)> InventorySlotLamada) const
{
	for (const auto& TmpSlot : SlotArrayInventory->GetAllChildren())
	{
		if (UUI_InventorySlot* InventorySlot = Cast<UUI_InventorySlot>(TmpSlot))
		{
			if (!InventorySlotLamada(InventorySlot))
			{
				break; //如果InventorySlotLamada返回false，停止Call
			}
		}
	}
}
