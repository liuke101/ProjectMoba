// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Item/UI_Item.h"

#include "Components/TextBlock.h"
#include "Table/SlotAsset.h"

void UUI_Item::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_Item::UpdateSlot(const FSlotAsset* SlotAsset)
{
	Super::UpdateSlot(SlotAsset);
	
	if(SlotAsset)
	{
		SetItemName(SlotAsset->SlotName);
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
	Super::OnClickedWidget();
}

FReply UUI_Item::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 鼠标右键显示装备合成界面
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		if(!CallItemSynthesisDelegate.ExecuteIfBound(GetItemDataID()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CallItemSynthesisDelegate 未绑定"));
		}
	}

	return FReply::Handled();
}
