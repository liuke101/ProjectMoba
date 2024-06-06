// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/UI_SimpleInventorySlot.h"

#include "ProjectMoba/MiscData.h"

void UUI_SimpleInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_SimpleInventorySlot::UpdateSlot(const FSlotData& SlotData) const
{
	UpdateIcon(SlotData.SlotIcon);
	SetTextNumber(SlotNumber, SlotData.Number);
}
