// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusBar/MobaStatusBarUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMobaStatusBarUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMobaStatusBarUI::SetLevel(const int32 InLevel) const
{
	LevelBar->SetText(FText::FromString(FString::FromInt(InLevel)));
}

void UMobaStatusBarUI::SetName(const FString& InTitle) const
{
	NameBar->SetText(FText::FromString(InTitle));
}

void UMobaStatusBarUI::SetManaPercent(const float InPercent) const
{
	ManaBar->SetPercent(InPercent);
}
