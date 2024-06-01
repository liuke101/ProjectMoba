// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Item/UI_ItemSynthesis.h"

#include "Table/SlotAsset.h"
#include "Components/CanvasPanel.h"

void UUI_ItemSynthesis::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_ItemSynthesis::UpdateSlot(const FSlotAsset* SlotAsset)
{
	ItemCanvasPanel->ClearChildren();
}
