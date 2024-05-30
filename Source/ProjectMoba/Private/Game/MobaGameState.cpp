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

