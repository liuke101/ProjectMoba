// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_Panel.h"

#include "ThreadManage.h"
#include "Game/MobaGameState.h"

UUI_Panel::UUI_Panel(): PlayerID(INDEX_NONE)
{
}

void UUI_Panel::NativeConstruct()
{
	Super::NativeConstruct();
	
	//绑定 属性更新委托
	GThread::GetCoroutines().BindLambda(0.3f, [&]()
	{
		if(AMobaGameState* MobaGameState = GetMobaGameState())
		{
			MobaGameState->OnUpdateAttributeDelegate.AddUObject(this, &UUI_Panel::ResponseUpdateSlot);
		}
	});
}

void UUI_Panel::RequestUpdateSlot(const ECharacterAttributeType CharacterAttributeType)
{
	if(PlayerID == INDEX_NONE) return;

	//客户端通过操作UI请求更新->服务器执行更新并广播委托->UI监听委托并更新UI
	if(AMobaGameState* MobaGameState = GetMobaGameState())
	{
		MobaGameState->Server_RequestUpdateCharacterAttribute(PlayerID, CharacterAttributeType);
	}
}

void UUI_Panel::ResponseUpdateSlot(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType)
{
}
