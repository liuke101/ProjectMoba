// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/Drop/UI_NativeOnDrop.h"

#include "Blueprint/DragDropOperation.h"
#include "UI/Inventory/UI_InventorySlot.h"

bool UUI_NativeOnDrop::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool bDrop = false;

	if (UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (UUI_InventorySlot* MyInventorySlot = Cast<UUI_InventorySlot>(InDragDropOperation->Payload))
		{
			MyInventorySlot->UpdateSlot();

			bDrop = true;
		}
	}

	return bDrop;
}
