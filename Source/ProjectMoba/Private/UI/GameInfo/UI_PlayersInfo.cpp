// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_PlayersInfo.h"
#include "Components/HorizontalBox.h"
#include "UI/UI_Slot.h"

UUI_PlayersInfo::UUI_PlayersInfo(): PlayerID(INDEX_NONE)
{
}

void UUI_PlayersInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_PlayersInfo::UpdateSlot(const int64& InPlayerID, const TArray<int32>& InSlotsID)
{
	PlayerID = InPlayerID;

	TArray<int32> TmpSlotsID = InSlotsID;
	CallAllSlot<UUI_Slot>([&](UUI_Slot* InSlot)->bool
	{
		if (TmpSlotsID.Num())
		{
			InSlot->SetSlotID(TmpSlotsID.Pop());
			InSlot->UpdateSlot();
		}
		return true;
	});
}

UPanelWidget* UUI_PlayersInfo::GetSlotLayoutPanel()
{
	return SlotLayoutPanel;
}
