// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_Panel.h"

#include "ThreadManage.h"
#include "Game/MobaGameState.h"

UUI_Panel::UUI_Panel()
{
	
}

void UUI_Panel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_Panel::BindDelegate()
{
	if(MobaGameState)
	{
		MobaGameState->OnUpdateAttributeDelegate.AddUObject(this, &UUI_Panel::ResponseUpdateSlot);
	}
	else
	{
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			BindDelegate();
		});
	}
}

void UUI_Panel::RequestUpdateSlot(const ECharacterAttributeType CharacterAttributeType)
{
	if(PlayerID == INDEX_NONE) return;
	
	//客户端通过操作UI请求更新->服务器执行更新并广播委托->UI监听委托并更新UI
	if(MobaGameState)
	{
		MobaGameState->RequestUpdateCharacterAttribute(PlayerID, PlayerID, CharacterAttributeType);
	}
}

void UUI_Panel::ResponseUpdateSlot(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType)
{
	
}

void UUI_Panel::ResponseUpdateSlots(int64 InPlayerID)
{
}
