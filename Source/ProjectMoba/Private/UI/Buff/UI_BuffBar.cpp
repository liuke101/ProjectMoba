// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Buff/UI_BuffBar.h"

#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "UI/Buff/UI_BuffSlot.h"


UUI_BuffBar::UUI_BuffBar()
{
	
}

void UUI_BuffBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_BuffBar::UpdateCD(int32 InSlotID, float InMaxCD) const
{
	if (BuffSlotClass && InSlotID != INDEX_NONE && InMaxCD != 0.f)
	{
		if (UUI_BuffSlot* BuffSlot = CreateWidget<UUI_BuffSlot>(GetWorld(), BuffSlotClass))
		{
			if (UWrapBoxSlot *InWrapBox = BuffBox->AddChildToWrapBox(BuffSlot))
			{
				InWrapBox->SetPadding(1.f);
				BuffSlot->UpdateCD(InSlotID, InMaxCD);
			}
		}
	}
}

void UUI_BuffBar::UpdateCD(int64 InPlayerID, int32 InSlotID, float InMaxCD) const
{
	if(InPlayerID == PlayerID)
	{
		UpdateCD(InSlotID, InMaxCD);
	}
}

void UUI_BuffBar::Clear()
{
	if(BuffBox)
	{
		BuffBox->ClearChildren();
	}
}

