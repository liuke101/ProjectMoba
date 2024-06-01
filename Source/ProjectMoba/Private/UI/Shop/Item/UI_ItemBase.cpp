// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Item/UI_ItemBase.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Game/MobaPlayerState.h"
#include "Table/SlotAsset.h"

void UUI_ItemBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemButton->OnClicked.AddDynamic(this, &UUI_ItemBase::OnClickedWidget);
}

void UUI_ItemBase::SetIcon(UTexture2D* InIcon2D)
{
	ItemIcon->SetBrushFromTexture(InIcon2D);
}

void UUI_ItemBase::OnClickedWidget()
{
	// 点击购买
	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		MobaPlayerState->Server_Buy(GetItemDataID());
	}
}

void UUI_ItemBase::UpdateSlot(const FSlotAsset* SlotAsset)
{
	if(SlotAsset)
	{
		SetItemDataID(SlotAsset->DataID);
		SetIcon(SlotAsset->SlotIcon);
	}
}
