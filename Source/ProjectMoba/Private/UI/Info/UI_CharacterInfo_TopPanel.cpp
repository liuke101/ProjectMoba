// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Info/UI_CharacterInfo_TopPanel.h"

#include "ThreadManage.h"
#include "Components/UniformGridPanel.h"
#include "Game/MobaGameState.h"
#include "Game/MobaPlayerState.h"
#include "UI/Info/UI_CharacterInfo.h"

void UUI_CharacterInfo_TopPanel::NativeConstruct()
{
	Super::NativeConstruct();

	//绑定委托
	GThread::GetCoroutines().BindLambda(0.3f, [&]()
	{
		//ID绑定委托
		if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
		{
			MobaPlayerState->BindPlayerIDDelegate.BindLambda([&](int64 InPlayerID)
			{
				CharacterInfo->SetPlayerID(InPlayerID);
			});
		}

		// 属性更新委托
		if(AMobaGameState* MobaGameState = GetMobaGameState())
		{
			MobaGameState->OnUpdateAllAttributesDelegate.AddUObject(CharacterInfo, &UUI_CharacterInfo::ResponseUpdateSlots);
		}
	});
}

void UUI_CharacterInfo_TopPanel::InitSlotLayout()
{
	Super::InitSlotLayout();
}

UPanelWidget* UUI_CharacterInfo_TopPanel::GetSlotLayoutPanel()
{
	return SlotArrayInventory;
}
