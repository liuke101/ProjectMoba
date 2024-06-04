﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameState.h"

#include "ThreadManage.h"
#include "Common/MethodUnit.h"
#include "Component/PlayerDataComponent.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "ProjectMoba/MiscData.h"
#include "Table/CharacterAsset.h"

AMobaGameState::AMobaGameState()
{
	
}

void AMobaGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		// 延迟执行，等待客户端生成
		GThread::GetCoroutines().BindLambda(2.0f,[&]()
		{
			//调用玩家的PlayerState，请求更新属性
			MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(),[&](AMobaPlayerState* MobaPlayerState)-> MethodUnit::EServerCallType
			{
				Server_RequestUpdateCharacterAttribute(MobaPlayerState->GetPlayerDataComponent()->PlayerID, ECharacterAttributeType::ECAT_None);
				return MethodUnit::EServerCallType::ECT_ProgressComplete;
			});
		});
	}
}


void AMobaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMobaGameState, PlayerLocations);
}


const TArray<FCharacterAsset*>* AMobaGameState::GetCachedCharacterAssets()
{
	if(CachedCharacterAssets.IsEmpty())
	{
		checkf(DT_CharacterAsset, TEXT("CharacterAssetTable为空，请在BP_MobaGameState中配置"));
		DT_CharacterAsset->GetAllRows(TEXT("Character Table"), CachedCharacterAssets);
	}

	return &CachedCharacterAssets;
}

const TArray<FCharacterAttribute*>* AMobaGameState::GetCachedCharacterAttributes()
{
	if(CachedCharacterAttributes.IsEmpty())
	{
		checkf(DT_CharacterAttribute, TEXT("CharacterAttributeTable为空，请在BP_MobaGameState中配置"));
		DT_CharacterAttribute->GetAllRows(TEXT("Character Attribute"), CachedCharacterAttributes);
	}

	return &CachedCharacterAttributes;
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
	for(auto Asset : *GetCachedCharacterAssets())
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
	for(auto Asset : *GetCachedCharacterAssets())
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
	for(auto Attribute : *GetCachedCharacterAttributes())
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
	return CharacterAttributes.Find(PlayerID);
	// for(auto& MAP : CharacterAttributes)
	// {
	// 	if(MAP.Key == PlayerID)
	// 	{
	// 		return &MAP.Value;
	// 	}
	// }
	// return nullptr;
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

		//调用玩家的PlayerState，更新属性
		MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(),[&](AMobaPlayerState* MobaPlayerState)-> MethodUnit::EServerCallType
		{
			if(MobaPlayerState->GetPlayerDataComponent()->PlayerID == PlayerID)
			{
				//响应更新属性
				switch (CharacterAttributeType)
				{
				case ECharacterAttributeType::ECAT_None:
					//ECAT_NONE代表更新整包
					MobaPlayerState->Client_ResponseUpdateAllCharacterAttributes(PlayerID, CharacterAttribute);
					break;
				case ECharacterAttributeType::ECAT_Level:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.Level);
					break;
				case ECharacterAttributeType::ECAT_MaxHealth:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MaxHealth);
					break;
				case ECharacterAttributeType::ECAT_CurrentHealth:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.CurrentHealth);
					break;
				case ECharacterAttributeType::ECAT_MaxMana:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MaxMana);
					break;
				case ECharacterAttributeType::ECAT_CurrentMana:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.CurrentMana);
					break;
				case ECharacterAttributeType::ECAT_PhysicalAttack:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.PhysicalAttack);
					break;
				case ECharacterAttributeType::ECAT_Armor:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.Armor);
					break;
				case ECharacterAttributeType::ECAT_PhysicalPenetration:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.PhysicalPenetration);
					break;
				case ECharacterAttributeType::ECAT_MagicAttack:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MagicAttack);
					break;
				case ECharacterAttributeType::ECAT_MagicResistance:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MagicResistance);
					break;
				case ECharacterAttributeType::ECAT_MagicPenetration:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MagicPenetration);
					break;
				case ECharacterAttributeType::ECAT_WalkSpeed:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.WalkSpeed);
					break;
				case ECharacterAttributeType::ECAT_AttackSpeed:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.AttackSpeed);
					break;
				case ECharacterAttributeType::ECAT_MaxEXP:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.MaxExp);
					break;
				case ECharacterAttributeType::ECAT_CurrentEXP:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.CurrentExp);
					break;
				case ECharacterAttributeType::ECAT_CriticalRate:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(PlayerID, CharacterAttributeType, CharacterAttribute.CriticalRate);
					break;
				}
				
				return MethodUnit::EServerCallType::ECT_ProgressComplete;
			}
			return MethodUnit::EServerCallType::ECT_InProgress;
		});
		
		
	}
	
	
}

void AMobaGameState::ResponseUpdateCharacterAttribute(int64 PlayerID, const ECharacterAttributeType CharacterAttributeType, float Value)
{
	if(PlayerID == INDEX_NONE) return;

	//获取PlayerID对应的属性，如果不存在则创建一个新的属性
	FCharacterAttribute* CharacterAttribute = GetCharacterAttributeFromPlayerID(PlayerID);
	if (!CharacterAttribute)
	{
		CharacterAttribute = new FCharacterAttribute();
		CharacterAttributes.Add(PlayerID, *CharacterAttribute);
	}

	//缓存属性数据
	switch (CharacterAttributeType) {
	case ECharacterAttributeType::ECAT_Level:
		CharacterAttribute->Level = Value;
		break;
	case ECharacterAttributeType::ECAT_MaxHealth:
		CharacterAttribute->MaxHealth = Value;
		break;
	case ECharacterAttributeType::ECAT_CurrentHealth:
		CharacterAttribute->CurrentHealth = Value;
		break;
	case ECharacterAttributeType::ECAT_MaxMana:
		CharacterAttribute->MaxMana = Value;
		break;
	case ECharacterAttributeType::ECAT_CurrentMana:
		CharacterAttribute->CurrentMana = Value;
		break;
	case ECharacterAttributeType::ECAT_PhysicalAttack:
		CharacterAttribute->PhysicalAttack = Value;
		break;
	case ECharacterAttributeType::ECAT_Armor:
		CharacterAttribute->Armor = Value;
		break;
	case ECharacterAttributeType::ECAT_PhysicalPenetration:
		CharacterAttribute->PhysicalPenetration = Value;	
		break;
	case ECharacterAttributeType::ECAT_MagicAttack:
		CharacterAttribute->MagicAttack = Value;
		break;
	case ECharacterAttributeType::ECAT_MagicResistance:
		CharacterAttribute->MagicResistance = Value;
		break;
	case ECharacterAttributeType::ECAT_MagicPenetration:
		CharacterAttribute->MagicPenetration = Value;
		break;
	case ECharacterAttributeType::ECAT_WalkSpeed:
		CharacterAttribute->WalkSpeed = Value;
		break;
	case ECharacterAttributeType::ECAT_AttackSpeed:
		CharacterAttribute->AttackSpeed = Value;
		break;
	case ECharacterAttributeType::ECAT_MaxEXP:
		CharacterAttribute->MaxExp = Value;
		break;
	case ECharacterAttributeType::ECAT_CurrentEXP:
		CharacterAttribute->CurrentExp = Value;
		break;
	case ECharacterAttributeType::ECAT_CriticalRate:
		CharacterAttribute->CriticalRate = Value;
		break;
	default: ;
	}
	
	//广播委托，更新UI
	OnUpdateAttributeDelegate.Broadcast(PlayerID, CharacterAttributeType);
}

void AMobaGameState::ResponseUpdateAllCharacterAttributes(int64 PlayerID,
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
	if(!OnUpdateAllAttributesDelegate.ExecuteIfBound(PlayerID))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnUpdateAllAttributesDelegate未绑定"));
	}
}
