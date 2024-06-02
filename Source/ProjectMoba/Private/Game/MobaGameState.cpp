// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameState.h"

#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "ProjectMoba/MiscData.h"
#include "Table/CharacterAsset.h"

AMobaGameState::AMobaGameState()
{
	
}


void AMobaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMobaGameState, PlayerLocations);
}


const TArray<FCharacterAsset*>* AMobaGameState::GetCharacterAssets()
{
	if(CacheCharacterAssets.IsEmpty())
	{
		checkf(DT_CharacterAsset, TEXT("CharacterAssetTable为空，请在BP_MobaGameState中配置"));
		DT_CharacterAsset->GetAllRows(TEXT("Character Table"), CacheCharacterAssets);
	}

	return &CacheCharacterAssets;
}

const TArray<FCharacterAttribute*>* AMobaGameState::GetCharacterAttributes()
{
	if(CacheCharacterAttributes.IsEmpty())
	{
		checkf(DT_CharacterAttribute, TEXT("CharacterAttributeTable为空，请在BP_MobaGameState中配置"));
		DT_CharacterAttribute->GetAllRows(TEXT("Character Attribute"), CacheCharacterAttributes);
	}

	return &CacheCharacterAttributes;
}


void AMobaGameState::AddCharacterAttribute(const int64 PlayerID, const int32 CharacterID)
{
	if (!CharacterAttributes.Contains(PlayerID))
	{
		CharacterAttributes.Add(PlayerID, *GetCharacterAttributeFromCharacterID(CharacterID));
		CharacterAttributes[PlayerID].ResetAttribute();
	}
}

const FCharacterAsset* AMobaGameState::GetCharacterAssetFromCharacterID(const int32 CharacterID)
{
	for(auto Asset : *GetCharacterAssets())
	{
		if(Asset->DataID == CharacterID)
		{
			return Asset;
		}
	}

	return nullptr;
}

const FCharacterAsset* AMobaGameState::GetCharacterAssetFromPlayerID(const int64 PlayerID)
{
	for(auto Asset : *GetCharacterAssets())
	{
		if(Asset->DataID == GetCharacterIDFromPlayerID(PlayerID))
		{
			return Asset;
		}
	}

	return nullptr;
}

const FCharacterAttribute* AMobaGameState::GetCharacterAttributeFromCharacterID(const int32 CharacterID)
{
	for(auto Attribute : *GetCharacterAttributes())
	{
		if(Attribute->DataID == CharacterID)
		{
			return Attribute;
		}
	}
	return nullptr;
}

FCharacterAttribute* AMobaGameState::GetCharacterAttributeFromPlayerID(const int64 PlayerID)
{
	for(auto& MAP : CharacterAttributes)
	{
		if(MAP.Key == PlayerID)
		{
			return &MAP.Value;
		}
	}
	return nullptr;
}

void AMobaGameState::UpdateCharacterLocation(const int64 PlayerID, const FVector& Location)
{
	for(auto& PlayerLocation : PlayerLocations)
	{
		if(PlayerLocation.PlayerID == PlayerID)
		{
			PlayerLocation.Location = Location;
			break;
		}
	}
}

void AMobaGameState::AddCharacterLocation(const int64 PlayerID, const FVector& Location)
{
	for(auto PlayerLocation : PlayerLocations)
	{
		if(PlayerLocation.PlayerID == PlayerID)
		{
			return;
		}
	}
	// 如果没有找到，就添加
	PlayerLocations.Add(FPlayerLocation(PlayerID, Location));
}

bool AMobaGameState::GetCharacterLocation(const int64 PlayerID, FVector& OutLocation) const
{
	for(auto PlayerLocation : PlayerLocations)
	{
		if(PlayerLocation.PlayerID == PlayerID)
		{
			OutLocation = PlayerLocation.Location;
			return true;
		}
	}
	return false;
}

int32 AMobaGameState::GetCharacterIDFromPlayerID(const int64 PlayerID) 
{
	if(const FCharacterAttribute* CharacterAttribute = GetCharacterAttributeFromPlayerID(PlayerID))
	{
		return CharacterAttribute->DataID;
	}
	return INDEX_NONE;
}

void AMobaGameState::Server_RequestUpdateCharacterAttribute_Implementation(int64 PlayerID, const ECharacterAttributeType CharacterAttributeType)
{
	if(PlayerID == INDEX_NONE) return;

	if(CharacterAttributes.Contains(PlayerID))
	{
		//获取角色属性
		const FCharacterAttribute& CharacterAttribute = CharacterAttributes[PlayerID];

		//响应更新属性
		switch (CharacterAttributeType)
		{
		case ECharacterAttributeType::ECAT_None:
			//ECAT_NONE代表更新整包
			Server_ResponseUpdateAllCharacterAttributes(PlayerID, CharacterAttribute);
			break;
		case ECharacterAttributeType::ECAT_Level:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.Level);
			break;
		case ECharacterAttributeType::ECAT_MaxHealth:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MaxHealth);
			break;
		case ECharacterAttributeType::ECAT_CurrentHealth:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.CurrentHealth);
			break;
		case ECharacterAttributeType::ECAT_MaxMana:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MaxMana);
			break;
		case ECharacterAttributeType::ECAT_CurrentMana:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.CurrentMana);
			break;
		case ECharacterAttributeType::ECAT_PhysicalAttack:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.PhysicalAttack);
			break;
		case ECharacterAttributeType::ECAT_Armor:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.Armor);
			break;
		case ECharacterAttributeType::ECAT_PhysicalPenetration:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.PhysicalPenetration);
			break;
		case ECharacterAttributeType::ECAT_MagicAttack:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MagicAttack);
			break;
		case ECharacterAttributeType::ECAT_MagicResistance:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MagicResistance);
			break;
		case ECharacterAttributeType::ECAT_MagicPenetration:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MagicPenetration);
			break;
		case ECharacterAttributeType::ECAT_WalkSpeed:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.WalkSpeed);
			break;
		case ECharacterAttributeType::ECAT_AttackSpeed:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.AttackSpeed);
			break;
		case ECharacterAttributeType::ECAT_MaxEXP:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MaxEXP);
			break;
		case ECharacterAttributeType::ECAT_CurrentEXP:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.CurrentEXP);
			break;
		case ECharacterAttributeType::ECAT_CriticalRate:
			Server_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.CriticalRate);
			break;
		}
	}
	
	
}

void AMobaGameState::Server_ResponseUpdateCharacterAttribute_Implementation(int64 PlayerID, const ECharacterAttributeType CharacterAttributeType, float Value)
{
	//不需要缓存属性数据，直接广播委托，更新UI
	OnUpdateAttributeDelegate.Broadcast(PlayerID, CharacterAttributeType, Value);
}

void AMobaGameState::Server_ResponseUpdateAllCharacterAttributes_Implementation(int64 PlayerID,
	const FCharacterAttribute& CharacterAttribute)
{
	// 缓存属性数据
	if(CharacterAttributes.Contains(PlayerID))
	{
		CharacterAttributes[PlayerID] = CharacterAttribute;
	}
	else
	{
		CharacterAttributes.Add(PlayerID, CharacterAttribute);
	}

	// 广播委托，更新UI
	OnUpdateAllAttributesDelegate.Broadcast(PlayerID);
}
