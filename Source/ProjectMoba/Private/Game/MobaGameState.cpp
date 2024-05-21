// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameState.h"

#include "MiscData.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "Table/CharacterAttributeTable.h"
#include "Table/CharacterAssetTable.h"

AMobaGameState::AMobaGameState()
{
	AddCharacterLocation(0, FVector::ZeroVector);
	AddCharacterLocation(1, FVector::ZeroVector);
}


void AMobaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMobaGameState, PlayerLocations);
}


const TArray<FCharacterAssetTable*>* AMobaGameState::GetCharacterAssetTables()
{
	if(CacheCharacterAssetTables.IsEmpty())
	{
		checkf(CharacterAssetTablePtr, TEXT("CharacterAssetTable为空，请在BP_MobaGameState中配置"));
		CharacterAssetTablePtr->GetAllRows(TEXT("Character Table"), CacheCharacterAssetTables);
	}

	return &CacheCharacterAssetTables;
}

const TArray<FCharacterAttributeTable*>* AMobaGameState::GetCharacterAttributeTables()
{
	if(CacheCharacterAttributeTables.IsEmpty())
	{
		checkf(CharacterAttributeTablePtr, TEXT("CharacterAttributeTable为空，请在BP_MobaGameState中配置"));
		CharacterAttributeTablePtr->GetAllRows(TEXT("Character Attribute"), CacheCharacterAttributeTables);
	}

	return &CacheCharacterAttributeTables;
}

const FCharacterAssetTable* AMobaGameState::GetCharacterAssetTable(const int32& InID)
{
	for(auto Table : *GetCharacterAssetTables())
	{
		if(Table->CharacterID == InID)
		{
			return Table;
		}
	}

	return nullptr;
}

const FCharacterAttributeTable* AMobaGameState::GetCharacterAttributeTable(const int32& InID)
{
	for(auto Table : *GetCharacterAttributeTables())
	{
		if(Table->CharacterID == InID)
		{
			return Table;
		}
	}
	return nullptr;
}

void AMobaGameState::UpdateCharacterLocation(const int32 InID, const FVector& InLocation)
{
	for(auto& PlayerLocation : PlayerLocations)
	{
		if(PlayerLocation.CharacterID == InID)
		{
			PlayerLocation.Location = InLocation;
			break;
		}
	}
}

void AMobaGameState::AddCharacterLocation(const int32 InID, const FVector& InLocation)
{
	for(auto PlayerLocation : PlayerLocations)
	{
		if(PlayerLocation.CharacterID == InID)
		{
			return;
		}
	}
	// 如果没有找到，就添加
	PlayerLocations.Add(FPlayerLocation(InID, InLocation));
}

bool AMobaGameState::GetCharacterLocation(const int32 InID, FVector& OutLocation) const
{
	for(auto PlayerLocation : PlayerLocations)
	{
		if(PlayerLocation.CharacterID == InID)
		{
			OutLocation = PlayerLocation.Location;
			return true;
		}
	}
	return false;
}

