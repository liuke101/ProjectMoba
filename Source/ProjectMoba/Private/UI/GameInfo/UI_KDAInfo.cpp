// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_KDAInfo.h"
#include "Components/TextBlock.h"
#include "ProjectMoba/MiscData.h"

void UUI_KDAInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_KDAInfo::UpdateSlot(const FPlayerKDANetPackage& PlayerKDANetPackage) const
{
	KDAInfoText->SetText(FText::FromString(FString::Printf(TEXT("%2d/%2d/%2d"), PlayerKDANetPackage.KillNum, PlayerKDANetPackage.DeathNum, PlayerKDANetPackage.AssistNum)));
	
	MinionKillNumber->SetText(FText::FromString(FString::FromInt(PlayerKDANetPackage.MinionKillNum)));
}
