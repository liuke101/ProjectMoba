// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Tip/UI_Tip.h"

#include "Components/RichTextBlock.h"

void UUI_Tip::NativeConstruct()
{
	Super::NativeConstruct();

	//自动换行
	RichTextAction->SetAutoWrapText(true);
	RichTextActive->SetAutoWrapText(true);
	RichTextPassive->SetAutoWrapText(true);
}

void UUI_Tip::SetRichTextAction(const FText& InText) const
{
	RichTextAction->SetText(InText);
}

void UUI_Tip::SetRichTextActive(const FText& InText) const
{
	RichTextActive->SetText(InText);
}

void UUI_Tip::SetRichTextPassive(const FText& InText) const
{
	RichTextPassive->SetText(InText);
}
