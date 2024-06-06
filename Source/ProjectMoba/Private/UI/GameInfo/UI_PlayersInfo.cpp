// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_PlayersInfo.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectMoba/MiscData.h"
#include "UI/UI_Slot.h"
#include "UI/GameInfo/UI_KillInfo.h"
#include "UI/Inventory/UI_SimpleInventorySlot.h"

UUI_PlayersInfo::UUI_PlayersInfo(): PlayerID(INDEX_NONE)
{
}

void UUI_PlayersInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_PlayersInfo::UpdateSlot(const FPlayerTeamNetPackage& PlayerTeamNetPackage)
{
	//解析玩家信息
	PlayerID = PlayerTeamNetPackage.PlayerInfoNetPackage.PlayerID;
	CharacterIcon->SetBrushFromTexture(PlayerTeamNetPackage.PlayerInfoNetPackage.PlayerIcon);
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("%2d"), PlayerTeamNetPackage.PlayerInfoNetPackage.CharacterLevel)));
	
	//解析击杀信息
	KillInfo->UpdateSlot(PlayerTeamNetPackage.PlayerInfoNetPackage);
	
	//解析背包信息
	int i = 0;
	CallAllSlot<UUI_SimpleInventorySlot>([&](UUI_SimpleInventorySlot* InSlot)->bool
	{
		InSlot->SetSlotID(PlayerTeamNetPackage.SlotDataNetPackage.SlotIDs[i]);
		InSlot->UpdateSlot(PlayerTeamNetPackage.SlotDataNetPackage.SlotDatas[i]);
		return true;
	});
}


UPanelWidget* UUI_PlayersInfo::GetSlotLayoutPanel()
{
	return SlotLayoutPanel;
}
