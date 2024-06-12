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

void UUI_BuffBar::UpdateCD(int32 InSlotID, float InMaxCD)
{
	if (BuffSlotClass && InSlotID != INDEX_NONE && InMaxCD != 0.f)
	{
		if (UUI_BuffSlot* BuffSlot = CreateWidget<UUI_BuffSlot>(GetWorld(), BuffSlotClass))
		{
			if (UWrapBoxSlot *InWrapBox = BuffBox->AddChildToWrapBox(BuffSlot))
			{
				BuffSlot->SetSlotID(InSlotID);  //TODO: 这里实际传进来的是DataID，是否要区分？
				InWrapBox->SetPadding(1.f);
				BuffSlot->UpdateCD(InSlotID, InMaxCD);
			}
		}
	}
}

void UUI_BuffBar::UpdateCD(int64 InPlayerID, int32 InSlotID, float InMaxCD) 
{
	if(InPlayerID == PlayerID)
	{
		bool bFind = false;
		CallAllSlot<UUI_BuffSlot>([&](UUI_BuffSlot* InSlot)
		{
			if(InSlot->GetSlotID() == InSlotID)
			{
				InSlot->SetCurrentBuffCD(InMaxCD);
				bFind = true;
				return false;
			}
			
			return false;
		});

		if(!bFind)
		{
			UpdateCD(InSlotID, InMaxCD);
		}
	}
}

void UUI_BuffBar::RemoveCD(int64 InPlayerID, int32 InSlotID)
{
	if(InPlayerID == PlayerID)
	{
		CallAllSlot<UUI_BuffSlot>([&](UUI_BuffSlot* InSlot)->bool
		{
			if(InSlot->GetSlotID() == InSlotID)
			{
				InSlot->SetCurrentBuffCD(0.0f);
				return false;
			}
			return true;
		});
	}
}

void UUI_BuffBar::Clear() const
{
	if(BuffBox)
	{
		BuffBox->ClearChildren();
	}
}

void UUI_BuffBar::InitSlotLayout(const TArray<int32>& SlotIDs)
{
	Super::InitSlotLayout(SlotIDs);
}

UPanelWidget* UUI_BuffBar::GetSlotLayoutPanel()
{
	return BuffBox;
}

