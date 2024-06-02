// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Info/UI_CharacterInfo.h"

#include "ThreadManage.h"
#include "Components/TextBlock.h"
#include "Game/MobaGameState.h"
#include "ProjectMoba/MobaType.h"

UUI_CharacterInfo::UUI_CharacterInfo()
	: PlayerID(INDEX_NONE)
{
}

void UUI_CharacterInfo::NativeConstruct()
{
	Super::NativeConstruct();

	//绑定 属性更新委托
	GThread::GetCoroutines().BindLambda(0.3f, [&]()
	{
		if(AMobaGameState* MobaGameState = GetMobaGameState())
		{
			MobaGameState->OnUpdateAttributeDelegate.AddUObject(this, &UUI_CharacterInfo::ResponseUpdateSlot);
		}
	});
}

void UUI_CharacterInfo::RequestUpdateSlot(const ECharacterAttributeType CharacterAttributeType)
{
	if(PlayerID == INDEX_NONE) return;

	//客户端通过操作UI请求更新->服务器执行更新并广播委托->UI监听委托并更新UI
	if(AMobaGameState* MobaGameState = GetMobaGameState())
	{
		MobaGameState->Server_RequestUpdateCharacterAttribute(PlayerID, CharacterAttributeType);
	}
}

void UUI_CharacterInfo::ResponseUpdateSlot(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType, float Value)
{
	if(InPlayerID != this->PlayerID) return;
	
	//枚举值看做协议，根据传来的协议值更新UI
	switch (CharacterAttributeType)
	{
	case ECharacterAttributeType::ECAT_PhysicalAttack:
		PhysicalAttack->SetText(FText::FromString(FString::FromInt(static_cast<int>(Value))));
		break;
	case ECharacterAttributeType::ECAT_Armor:
		Armor->SetText(FText::FromString(FString::FromInt(static_cast<int>(Value))));
		break;
	case ECharacterAttributeType::ECAT_PhysicalPenetration:
		PhysicalPenetration->SetText(FText::FromString(FString::FromInt(static_cast<int>(Value))));
		break;
	case ECharacterAttributeType::ECAT_AttackSpeed:
		AttackSpeed->SetText(FText::FromString(FString::SanitizeFloat(Value, 1)));
		break;
	case ECharacterAttributeType::ECAT_MagicAttack:
		MagicAttack->SetText(FText::FromString(FString::FromInt(static_cast<int>(Value))));
	case ECharacterAttributeType::ECAT_MagicResistance:
		MagicResistance->SetText(FText::FromString(FString::FromInt(static_cast<int>(Value))));
		break;
	case ECharacterAttributeType::ECAT_MagicPenetration:
		MagicPenetration->SetText(FText::FromString(FString::FromInt(static_cast<int>(Value))));
		break;
	case ECharacterAttributeType::ECAT_WalkSpeed:
		WalkSpeed->SetText(FText::FromString(FString::FromInt(static_cast<int>(Value))));
		break;
	default: ;
	}
}

