// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusBar/UI_StatusBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUI_StatusBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_StatusBar::SetLevel(const int32 InLevel) const
{
	LevelBar->SetText(FText::FromString(FString::FromInt(InLevel)));
}

void UUI_StatusBar::SetName(const FString& InTitle) const
{
	NameBar->SetText(FText::FromString(InTitle));
}

void UUI_StatusBar::SetManaPercent(const float InPercent) const
{
	ManaBar->SetPercent(InPercent);
}
