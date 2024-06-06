// Fill out your copyright notice in the Description page of Project Settings.


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
		GThread::GetCoroutines().BindLambda(4.0f,[&]()
		{
			//调用玩家的PlayerState，请求更新属性
			MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(),[&](const AMobaPlayerState* MobaPlayerState)
			{
				Server_RequestUpdateCharacterAttribute(MobaPlayerState->GetPlayerID(),MobaPlayerState->GetPlayerID(), ECharacterAttributeType::ECAT_All);
				return MethodUnit::EServerCallType::ECT_InProgress;
			});
		});
		
		//绑定击杀系统函数
		BindKillFuntion();
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

void AMobaGameState::Server_RequestUpdateCharacterAttribute_Implementation(int64 PlayerID, int64 UpdatedPlayerID, const ECharacterAttributeType CharacterAttributeType)
{
	if(PlayerID == INDEX_NONE || UpdatedPlayerID == INDEX_NONE) return;

	if(CharacterAttributes.Contains(UpdatedPlayerID))
	{
		//获取待更新角色属性
		const FCharacterAttribute& CharacterAttribute = CharacterAttributes[UpdatedPlayerID];

		//调用玩家的PlayerState，更新属性
		MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(),[&](AMobaPlayerState* MobaPlayerState)-> MethodUnit::EServerCallType
		{
			if(MobaPlayerState->GetPlayerID() == PlayerID)
			{
				//响应更新属性
				switch (CharacterAttributeType)
				{
				case ECharacterAttributeType::ECAT_All:
					//ECAT_All代表更新整包
					MobaPlayerState->Client_ResponseUpdateAllCharacterAttributes(UpdatedPlayerID, CharacterAttribute);
					break;
				case ECharacterAttributeType::ECAT_Level:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.Level);
					break;
				case ECharacterAttributeType::ECAT_MaxHealth:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.MaxHealth);
					break;
				case ECharacterAttributeType::ECAT_CurrentHealth:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.CurrentHealth);
					break;
				case ECharacterAttributeType::ECAT_MaxMana:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.MaxMana);
					break;
				case ECharacterAttributeType::ECAT_CurrentMana:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.CurrentMana);
					break;
				case ECharacterAttributeType::ECAT_PhysicalAttack:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.PhysicalAttack);
					break;
				case ECharacterAttributeType::ECAT_Armor:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.Armor);
					break;
				case ECharacterAttributeType::ECAT_PhysicalPenetration:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.PhysicalPenetration);
					break;
				case ECharacterAttributeType::ECAT_MagicAttack:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.MagicAttack);
					break;
				case ECharacterAttributeType::ECAT_MagicResistance:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.MagicResistance);
					break;
				case ECharacterAttributeType::ECAT_MagicPenetration:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.MagicPenetration);
					break;
				case ECharacterAttributeType::ECAT_WalkSpeed:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.WalkSpeed);
					break;
				case ECharacterAttributeType::ECAT_AttackSpeed:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.AttackSpeed);
					break;
				case ECharacterAttributeType::ECAT_MaxEXP:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.MaxExp);
					break;
				case ECharacterAttributeType::ECAT_CurrentEXP:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.CurrentExp);
					break;
				case ECharacterAttributeType::ECAT_CriticalRate:
					MobaPlayerState->Client_ResponseUpdateCharacterAttribute(UpdatedPlayerID, CharacterAttributeType, CharacterAttribute.CriticalRate);
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
	OnUpdateAllAttributesDelegate.Broadcast(PlayerID);
}

void AMobaGameState::UpdateKillMessage(const FKillNetPackgae& KillNetPackgae) const
{
	//广播
	MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(),[&](AMobaPlayerState* MobaPlayerState)
	{
		MobaPlayerState->Client_UpdateKillMessage(KillNetPackgae);
		return MethodUnit::EServerCallType::ECT_InProgress;
	});
}

void AMobaGameState::SettleDeath(int64 KillerPlayerID, int64 KilledPlayerID)
{
	if(KillerPlayerID == KilledPlayerID) return;

	//击杀者和被击杀者的PlayerState，如果为nullptr则代表是AI
	AMobaPlayerState* KillerPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), KillerPlayerID);
	AMobaPlayerState* KilledPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), KilledPlayerID);
	
	/** 1 击杀者是玩家, 被击杀者是玩家 */
	if(KillerPlayerState && KilledPlayerState)
	{
		//调用击杀系统(在其中处理击杀信息)
		KillSystem.Kill(KillerPlayerID, KilledPlayerID);
		
		//记录玩家击杀数
		KillerPlayerState->GetPlayerDataComponent()->KillNum++;
		KilledPlayerState->GetPlayerDataComponent()->DeathNum++;
		//TODO:记录团队击杀数
		
		//助攻玩家记录助攻数，并获得奖励
		for(const auto& Assit : KilledPlayerState->GetAssistPlayers())
		{
			if(AMobaPlayerState* AssitPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), Assit.PlayerID))
			{
				AssitPlayerState->GetPlayerDataComponent()->AssistNum++;
			}
		}
	}
	/** 2 击杀者是玩家, 被击杀者是AI */
	else if(KillerPlayerState && !KilledPlayerState)
	{
		if(const FCharacterAsset* KillerCharacterAsset = GetCharacterAssetFromPlayerID(KillerPlayerID))
		{
			if(const FCharacterAsset* KilledCharacterAsset = GetCharacterAssetFromPlayerID(KilledPlayerID))
			{
				//如果击杀跑塔
				//如果被击杀者是炮塔，则奖励击杀塔的玩家，范围内的队友也获得奖励
				if(KilledCharacterAsset->CharacterType >= ECharacterType::ECT_1st_Tower && KilledCharacterAsset->CharacterType <= ECharacterType::ECT_Base_Tower)
				{
					//收集和更新击杀信息
					FKillNetPackgae KillNetPackgae;
					KillNetPackgae.KillerName = KillerPlayerState->GetPlayerDataComponent()->PlayerName;
					KillNetPackgae.KillerIcon = KillerCharacterAsset->CharacterIcon;
					KillNetPackgae.KilledName = KilledCharacterAsset->CharacterName;
					KillNetPackgae.KilledIcon = KilledCharacterAsset->CharacterIcon;
					UpdateKillMessage(KillNetPackgae);
				}
				//如果击杀小兵和野怪，记录补兵数
				else 
				{
					KillerPlayerState->GetPlayerDataComponent()->MinionKillNum++;
					//TOOD:金币奖励和UI提示
				}
			}
		}
	}
	/** 3 击杀者是AI, 被击杀者是玩家 */
	else if(!KillerPlayerState && KilledPlayerState)
	{
		//在一定时间有玩家助攻
		if(const FAssistPlayer* LastAssistPlayer = KilledPlayerState->GetLastAssistPlayer())
		{
			//找到最近助攻的玩家记录击杀数（记为玩家击杀玩家）
			if(const AMobaPlayerState* LastAssitPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), LastAssistPlayer->PlayerID))
			{
				//调用击杀系统(在其中处理击杀信息)
				KillSystem.Kill(LastAssistPlayer->PlayerID, KilledPlayerID);
				LastAssitPlayerState->GetPlayerDataComponent()->KillNum++;
				
				//其他助攻玩家记录助攻数
				for(const auto& Assit : KilledPlayerState->GetAssistPlayers())
				{
					if(Assit != *LastAssistPlayer)
					{
						if(const AMobaPlayerState* OtherAssitPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), Assit.PlayerID))
						{
							OtherAssitPlayerState->GetPlayerDataComponent()->AssistNum++;
						}
					}
				}
			}
		}
		//没有玩家助攻，则播放AI击杀玩家信息
		else
		{
			if(const FCharacterAsset* KillerCharacterAsset = GetCharacterAssetFromPlayerID(KillerPlayerID))
			{
				if(const FCharacterAsset* KilledCharacterAsset = GetCharacterAssetFromPlayerID(KilledPlayerID))
				{
					//收集和更新击杀信息
					FKillNetPackgae KillNetPackgae;
					KillNetPackgae.KillerName = KillerCharacterAsset->CharacterName;
					KillNetPackgae.KillerIcon = KillerCharacterAsset->CharacterIcon;
					KillNetPackgae.KilledName = KilledPlayerState->GetPlayerDataComponent()->PlayerName;
					KillNetPackgae.KilledIcon = KilledCharacterAsset->CharacterIcon;
					UpdateKillMessage(KillNetPackgae);
				}
			}
		}
		
	}
	/** 4 击杀者是AI, 被击杀者是AI */
	else if(!KillerPlayerState && !KilledPlayerState)
	{
		if(const FCharacterAsset* KillerCharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), KillerPlayerID))
		{
			if(const FCharacterAsset* KilledCharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), KilledPlayerID))
			{
				//如果AI击杀炮塔，则判断在一定时间内是否有玩家助攻
				//如果有玩家助攻，则找到最近助攻的玩家进行全额金币奖励，播放击杀信息。范围内的队友也获得奖励
				//如果没有玩家助攻，范围内的队友也获得奖励，播放AI击杀炮塔信息
				if(KilledCharacterAsset->CharacterType >= ECharacterType::ECT_1st_Tower && KilledCharacterAsset->CharacterType <= ECharacterType::ECT_Base_Tower)
				{
					//TODO: 因为KilledPlayerState为nullptr，所以无法获得助攻玩家列表

					//AI拆塔也要显示
					//收集和更新击杀信息
					FKillNetPackgae KillNetPackgae;
					KillNetPackgae.KillerName = KillerCharacterAsset->CharacterName;
					KillNetPackgae.KillerIcon = KillerCharacterAsset->CharacterIcon;
					KillNetPackgae.KilledName = KilledCharacterAsset->CharacterName;
					KillNetPackgae.KilledIcon = KilledCharacterAsset->CharacterIcon;
					UpdateKillMessage(KillNetPackgae);
				}
			}
		}
	}
}

bool AMobaGameState::IsPlayer(int64 PlayerID) const
{
	return MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), PlayerID) != nullptr;
}

void AMobaGameState::Death(int64 PlayerID)
{
	KillSystem.Death(PlayerID);
}

void AMobaGameState::BindKillFuntion()
{
	//TODO: 课程对三种击杀方式做了区分，这里暂时不区分
	KillSystem.NormalKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_NormalKill, KillerPlayerID, KilledPlayerID);
	};
	
	KillSystem.FirstBloodFuntion = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_FirstBlood, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.DoubleKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_DoubleKill, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.TripleKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_TripleKill, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.QuadraKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_QuadraKill, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.PentaKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_PentaKill, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.DaShaTeShaFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_DaShaTeSha, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.SuoXiangPiMiFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_SuoXiangPiMi, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.HunShenShiDanFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_HunShenShiDan, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.YongGuanSanJunFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_YongGuanSanJun, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.YiJiDangQianFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_YiJiDangQian, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.WanFuMoDiFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_WanFuMoDi, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.JuShiWuShuangFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_JuShiWuShuang, KillerPlayerID, KilledPlayerID);
	};

	KillSystem.TianXiaWuDiFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_TianXiaWuDi, KillerPlayerID, KilledPlayerID);
	};
}


void AMobaGameState::MulticastKillMessage(EKillType KillType, int64 KillerPlayerID, int64 KilledPlayerID)
{
	//收集击杀信息
	FKillNetPackgae KillNetPackgae;
	KillNetPackgae.KillType = KillType;
			
	if(const FCharacterAsset* KillerCharacterAsset = GetCharacterAssetFromPlayerID(KillerPlayerID))
	{
		KillNetPackgae.KillerIcon = KillerCharacterAsset->CharacterIcon;
	}

	if(const AMobaPlayerState* KillerPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), KillerPlayerID))
	{
		//注意获取的是玩家名字，而不是KillerCharacterAsset存的角色名
		KillNetPackgae.KillerName = KillerPlayerState->GetPlayerDataComponent()->PlayerName;
	}
			
	if(const FCharacterAsset* KilledCharacterAsset = GetCharacterAssetFromPlayerID(KilledPlayerID))
	{
		KillNetPackgae.KilledIcon = KilledCharacterAsset->CharacterIcon;
	}

	if(const AMobaPlayerState* KilledPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), KilledPlayerID))
	{
		KillNetPackgae.KillerName = KilledPlayerState->GetPlayerDataComponent()->PlayerName;
	}
	
	//击杀信息广播
	MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(),[&](AMobaPlayerState* MobaPlayerState)
	{
		MobaPlayerState->Client_UpdateKillMessage(KillNetPackgae);
		return MethodUnit::EServerCallType::ECT_InProgress;
	});
}
