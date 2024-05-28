// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Inventory/DragDrop/UI_ICODragDrog.h"

#include "Components/Image.h"

void UUI_ICODragDrog::DrawIcon(UTexture2D *InIcon) const
{
	Icon->SetBrushFromTexture(InIcon);
}
