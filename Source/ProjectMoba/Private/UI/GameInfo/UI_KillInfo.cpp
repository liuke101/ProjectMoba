// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_KillInfo.h"

#include "Components/TextBlock.h"
#include "ProjectMoba/MiscData.h"

void UUI_KillInfo::UpdateSlot(const FPlayerInfoNetPackage& PlayerInfoNetPackage) const
{
	KillInfoText->SetText(FText::FromString(FString::Printf(TEXT("%2d/%2d/%2d"), PlayerInfoNetPackage.PlayerKillInfoNetPackage.KillNum, PlayerInfoNetPackage.PlayerKillInfoNetPackage.DeathNum, PlayerInfoNetPackage.PlayerKillInfoNetPackage.AssistNum)));
	
	MinionKillNumber->SetText(FText::FromString(FString::FromInt(PlayerInfoNetPackage.PlayerKillInfoNetPackage.MinionKillNum)));
}
