// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterInfo/UI_CharacterInfo.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Game/MobaGameState.h"
#include "ProjectMoba/MobaType.h"
#include "Table/CharacterAsset.h"

void UUI_CharacterInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_CharacterInfo::ResponseUpdateSlot(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType)
{
	if(InPlayerID != GetPlayerID()) return;

	if(AMobaGameState* MobaGameState = GetMobaGameState())
	{
		if(FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(InPlayerID))
		{
			//枚举值看做协议，根据传来的协议值更新UI
			switch (CharacterAttributeType)
			{
			case ECharacterAttributeType::ECAT_PhysicalAttack:
				PhysicalAttack->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->PhysicalAttack))));
				break;
			case ECharacterAttributeType::ECAT_Armor:
				Armor->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->Armor))));
				break;
			case ECharacterAttributeType::ECAT_PhysicalPenetration:
				PhysicalPenetration->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->PhysicalPenetration))));
				break;
			case ECharacterAttributeType::ECAT_AttackSpeed:
				AttackSpeed->SetText(FText::FromString(FString::SanitizeFloat(CharacterAttribute->AttackSpeed, 1)));
				break;
			case ECharacterAttributeType::ECAT_CriticalRate:
				CriticalRate->SetText(FText::FromString(FString::SanitizeFloat(CharacterAttribute->CriticalRate, 1)));
				break;
			case ECharacterAttributeType::ECAT_MagicAttack:
				MagicAttack->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->MagicAttack))));
			case ECharacterAttributeType::ECAT_MagicResistance:
				MagicResistance->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->MagicResistance))));
				break;
			case ECharacterAttributeType::ECAT_MagicPenetration:
				MagicPenetration->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->MagicPenetration))));
				break;
			case ECharacterAttributeType::ECAT_WalkSpeed:
				WalkSpeed->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->WalkSpeed))));
				break;
			default: ;
			}
		}
	}
	
}

void UUI_CharacterInfo::ResponseUpdateSlots(int64 InPlayerID)
{
	SetPlayerID(InPlayerID);

	//根据PlayerID读取CharacterAttribute，更新UI
	if(AMobaGameState* MobaGameState = GetMobaGameState())
	{
		if(const FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(InPlayerID))
		{
			//更新属性
			PhysicalAttack->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->PhysicalAttack))));
			Armor->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->Armor))));
			PhysicalPenetration->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->PhysicalPenetration))));
			AttackSpeed->SetText(FText::FromString(FString::SanitizeFloat(CharacterAttribute->AttackSpeed, 1)));
			CriticalRate->SetText(FText::FromString(FString::SanitizeFloat(CharacterAttribute->CriticalRate, 1)));
			MagicAttack->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->MagicAttack))));
			MagicResistance->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->MagicResistance))));
			MagicPenetration->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->MagicPenetration))));
			WalkSpeed->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CharacterAttribute->WalkSpeed))));

			//更新头像
			if(const FCharacterAsset* CharacterAsset = MobaGameState->GetCharacterAssetFromPlayerID(InPlayerID))
			{
				CharacterIcon->SetBrushFromTexture(CharacterAsset->CharacterIcon);
			}
		}
	}
}

