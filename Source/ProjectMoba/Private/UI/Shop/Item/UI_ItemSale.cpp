// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Item/UI_ItemSale.h"

#include "Blueprint/DragDropOperation.h"
#include "Components/TextBlock.h"
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
		if (UUI_Item* MyInventorySlot = Cast<UUI_Item>(InDragDropOperation->Payload))
		{
			// MyInventorySlot->UpdateSlot();

			bDrop = true;
		}
	}

	return bDrop;
}

void UUI_ItemSale::SetSellText(const FText& InContent)
{
	SellText->SetText(InContent);
}

