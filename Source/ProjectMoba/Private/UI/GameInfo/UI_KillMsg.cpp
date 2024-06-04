// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_KillMsg.h"

void UUI_KillMsg::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UUI_KillMsg::UpdateSlot()
{
	if (UWidgetAnimation* InWidgetAnim = GetNameWidgetAnimation(TEXT("KillTargetAnim")))
	{
		PlayAnimation(InWidgetAnim);
	}
}
