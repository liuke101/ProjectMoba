// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/SlotAsset.h"

FSlotAsset::FSlotAsset()
	: SlotName("Default"),
	SlotIcon(nullptr),
	SlotGold(0),
	SlotDescription(FText::FromString("Default")),
	SlotType({ ESlotType::EST_All })
{
}
