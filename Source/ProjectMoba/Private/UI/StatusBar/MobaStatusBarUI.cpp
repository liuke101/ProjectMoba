// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusBar/MobaStatusBarUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMobaStatusBarUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMobaStatusBarUI::SetTitle(const FString& InTitle) const
{
	Title->SetText(FText::FromString(InTitle));
}

void UMobaStatusBarUI::SetManaPercent(const float InPercent) const
{
	ManaBar->SetPercent(InPercent);
}
