// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Item/UI_Item.h"

#include "Components/TextBlock.h"
#include "Game/MobaPlayerState.h"
#include "Table/SlotAsset.h"

void UUI_Item::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_Item::UpdateSlot(const FSlotAsset* SlotAsset)
{
	if(SlotAsset)
	{
		SetItemDataID(SlotAsset->DataID);
		SetItemName(SlotAsset->SlotName);
		SetIcon(SlotAsset->SlotIcon);
		SetItemGold(SlotAsset->SlotGold);
		SetItemIntroduction(SlotAsset->SlotIntroduction);
	}
}

void UUI_Item::SetItemIntroduction(const FText& InContent)
{
	ItemIntroduction->SetText(InContent);
}

void UUI_Item::SetItemName(const FName& InName)
{
	ItemName->SetText(FText::FromName(InName));
}

void UUI_Item::SetItemGold(const int32 InGold)
{
	Gold->SetText(FText::Format(NSLOCTEXT("UUI_Item","SetItemGold","{0}"), InGold));
}

void UUI_Item::OnClickedWidget()
{
	//如果当前Slot不在CD中
	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		MobaPlayerState->Server_Buy(GetItemDataID());
	}
}