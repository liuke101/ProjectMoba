// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_TeamInfo.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/GameInfo/UI_PlayersInfo.h"

void UUI_TeamInfo::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UUI_TeamInfo::SpawnPlayerInfo(bool bEnemy, const int64& InPlayerID, const TArray<int32>& InSlotsID) const
{
	if (UVerticalBox* InArray = (bEnemy ? EnemyTeam : FriendlyTeam))
	{
		if (UUI_PlayersInfo* SlotWidget = CreateWidget<UUI_PlayersInfo>(GetWorld(), PlayersInfoClass))
		{
			if (UVerticalBoxSlot* InBoxSlot = InArray->AddChildToVerticalBox(SlotWidget))
			{
				InBoxSlot->SetPadding(1.0f);
				SlotWidget->UpdateSlot(InPlayerID,InSlotsID);
			}
		}
	}
}
