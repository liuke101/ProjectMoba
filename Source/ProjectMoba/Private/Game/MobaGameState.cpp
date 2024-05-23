// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameState.h"

#include "MiscData.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "Table/CharacterAsset.h"

AMobaGameState::AMobaGameState()
{
	
}


void AMobaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMobaGameState, PlayerLocations);
}


const TArray<FCharacterAsset*>* AMobaGameState::GetCharacterAssetsTemplate()
{
	if(CacheCharacterAssets.IsEmpty())
	{
		checkf(DT_CharacterAsset, TEXT("CharacterAssetTable为空，请在BP_MobaGameState中配置"));
		DT_CharacterAsset->GetAllRows(TEXT("Character Table"), CacheCharacterAssets);
	}

	return &CacheCharacterAssets;
}

const TArray<FCharacterAttribute*>* AMobaGameState::GetCharacterAttributesTemplate()
{
	if(CacheCharacterAttributes.IsEmpty())
	{
		checkf(DT_CharacterAttribute, TEXT("CharacterAttributeTable为空，请在BP_MobaGameState中配置"));
		DT_CharacterAttribute->GetAllRows(TEXT("Character Attribute"), CacheCharacterAttributes);
	}

	return &CacheCharacterAttributes;
}


void AMobaGameState::Add_PlayerID_To_CharacterAttribute(const int64 InPlayerID, const int32 InCharacterID)
{
	if (!PlayerID_To_CharacterAttribute.Contains(InPlayerID))
	{
		PlayerID_To_CharacterAttribute.Add(InPlayerID, *GetCharacterAttributeFromCharacterID(InCharacterID));
		PlayerID_To_CharacterAttribute[InPlayerID].ResetAttribute();
	}
}

const FCharacterAsset* AMobaGameState::GetCharacterAssetFromCharacterID(const int32 InCharacterID)
{
	for(auto Asset : *GetCharacterAssetsTemplate())
	{
		if(Asset->CharacterID == InCharacterID)
		{
			return Asset;
		}
	}

	return nullptr;
}

const FCharacterAsset* AMobaGameState::GetCharacterAssetFromPlayerID(const int64 InPlayerID)
{
	for(auto Asset : *GetCharacterAssetsTemplate())
	{
		if(Asset->CharacterID == GetCharacterIDFromPlayerID(InPlayerID))
		{
			return Asset;
		}
	}

	return nullptr;
}

const FCharacterAttribute* AMobaGameState::GetCharacterAttributeFromCharacterID(const int32 InCharacterID)
{
	for(auto Attribute : *GetCharacterAttributesTemplate())
	{
		if(Attribute->CharacterID == InCharacterID)
		{
			return Attribute;
		}
	}
	return nullptr;
}

FCharacterAttribute* AMobaGameState::GetCharacterAttributeFromPlayerID(const int64 InPlayerID)
{
	for(auto& MAP : PlayerID_To_CharacterAttribute)
	{
		if(MAP.Key == InPlayerID)
		{
			return &MAP.Value;
		}
	}
	return nullptr;
}

void AMobaGameState::UpdateCharacterLocation(const int64 InPlayerID, const FVector& InLocation)
{
	for(auto& Location : PlayerLocations)
	{
		if(Location.playerID == InPlayerID)
		{
			Location.Location = InLocation;
			break;
		}
	}
}

void AMobaGameState::AddCharacterLocation(const int64 InPlayerID, const FVector& InLocation)
{
	for(auto Location : PlayerLocations)
	{
		if(Location.playerID == InPlayerID)
		{
			return;
		}
	}
	// 如果没有找到，就添加
	PlayerLocations.Add(FPlayerLocation(InPlayerID, InLocation));
}

bool AMobaGameState::GetCharacterLocation(const int64 InPlayerID, FVector& OutLocation) const
{
	for(auto Location : PlayerLocations)
	{
		if(Location.playerID == InPlayerID)
		{
			OutLocation = Location.Location;
			return true;
		}
	}
	return false;
}

int32 AMobaGameState::GetCharacterIDFromPlayerID(const int64 InPlayerID) 
{
	if(const FCharacterAttribute* CharacterAttribute = GetCharacterAttributeFromPlayerID(InPlayerID))
	{
		return CharacterAttribute->CharacterID;
	}
	return INDEX_NONE;
}

