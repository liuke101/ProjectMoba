// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Info/UI_CharacterInfo_TopPanel.h"

#include "Components/UniformGridPanel.h"

void UUI_CharacterInfo_TopPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_CharacterInfo_TopPanel::InitSlotLayout()
{
	Super::InitSlotLayout();
}

UPanelWidget* UUI_CharacterInfo_TopPanel::GetSlotLayoutPanel()
{
	return SlotArrayInventory;
}
