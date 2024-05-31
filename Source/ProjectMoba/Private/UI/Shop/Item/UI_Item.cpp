// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/Item/UI_Item.h"

#include "Components/TextBlock.h"

void UUI_Item::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_Item::SetItemIntroduction(const FText& InContent)
{
	ItemIntroduction->SetText(InContent);
}

void UUI_Item::SetItemName(const FString& InName)
{
	ItemName->SetText(FText::FromString(InName));
}

void UUI_Item::SetItemGold(const int32 InGold)
{
	Gold->SetText(FText::Format(NSLOCTEXT("UUI_Item","SetItemGold","{0}"), InGold));
}

void UUI_Item::OnClickedWidget()
{

}