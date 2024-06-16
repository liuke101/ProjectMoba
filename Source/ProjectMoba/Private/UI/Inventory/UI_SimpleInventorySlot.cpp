// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/UI_SimpleInventorySlot.h"

#include "Components/Image.h"
#include "ProjectMoba/MiscData.h"
#include "UI/Tip/UI_Tip.h"

void UUI_SimpleInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	
}

void UUI_SimpleInventorySlot::UpdateSlot(const FSlotData& SlotData) const
{
	UpdateIcon(SlotData.SlotIcon);
	SetTextNumber(SlotNumber, SlotData.Number);
}

void UUI_SimpleInventorySlot::ResetSlot()
{
	Super::ResetSlot();
	SetTextNumber(SlotNumber, INDEX_NONE);
	SetSlotID(INDEX_NONE);
}

void UUI_SimpleInventorySlot::BindToolTip()
{
	GetSlotIcon()->SetToolTip(GetTip());
	GetSlotIcon()->SetCursor(EMouseCursor::Hand);
}
