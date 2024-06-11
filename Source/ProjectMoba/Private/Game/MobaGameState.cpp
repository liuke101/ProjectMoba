// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaGameState.h"

#include "ThreadManage.h"
#include "Character/MobaCharacter.h"
#include "Character/Hero/MobaHeroCharacter.h"
#include "Character/Turret/MobaTurretCharacter.h"
#include "Common/MethodUnit.h"
#include "Component/MobaAssistSystemComponent.h"
#include "Component/MobaKillSystemComponent.h"
#include "Component/MobaMinionSystemComponent.h"
#include "Component/PlayerDataComponent.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "ProjectMoba/MiscData.h"
#include "Table/CharacterAsset.h"

AMobaGameState::AMobaGameState()
{
	MobaKillSystemComponent = CreateDefaultSubobject<UMobaKillSystemComponent>(TEXT("MobaKillSystemComponent"));
	
	MobaMinionSystemComponent = CreateDefaultSubobject<UMobaMinionSystemComponent>(TEXT("MobaMinionSystemComponent"));
}

void AMobaGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		TeamKillCount.Add(ETeamType::ETT_Red, 0);
		TeamKillCount.Add(ETeamType::ETT_Blue, 0);
		
		//绑定击杀系统函数
		BindKillFuntion();
	}
}

void AMobaGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GThread::Destroy();  //解决 SimpleTread 在编辑器中的奔溃问题
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

int32 AMobaGameState::GetSkillDataIDFromAnimMontage(const UAnimMontage* AnimMontage)
{
	int32 DataID = INDEX_NONE;
	for(auto& Asset : *GetCachedCharacterAssets())
	{
		if(Asset->W_Skill.SkillMontage == AnimMontage)
		{
			DataID = Asset->DataID;
		}
		else if(Asset->E_Skill.SkillMontage == AnimMontage)
		{
			DataID = Asset->DataID;
		}
		else if(Asset->R_Skill.SkillMontage == AnimMontage)
		{
			DataID = Asset->DataID;
		}
		else if(Asset->F_Skill.SkillMontage == AnimMontage)
		{
			DataID = Asset->DataID;
		}
	}
	return DataID;
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
	for(auto& PlayerLocation : PlayerLocations)
	{
		if(PlayerLocation.PlayerID == PlayerID)
		{
			return;
		}
	}
	// 如果没有找到，就添加
	PlayerLocations.Add(FPlayerLocation(PlayerID, Location));
}

void AMobaGameState::RemoveCharacterLocation(const int64 PlayerID)
{
	FPlayerLocation Temp;
	for(auto& PlayerLocation : PlayerLocations)
	{
		if(PlayerLocation.PlayerID == PlayerID)
		{
			Temp = PlayerLocation;
		}
	}
	PlayerLocations.Remove(Temp);
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

void AMobaGameState::RequestUpdateCharacterAttribute(int64 PlayerID, int64 UpdatedPlayerID, const ECharacterAttributeType CharacterAttributeType)
{
	if(PlayerID == INDEX_NONE || UpdatedPlayerID == INDEX_NONE) return;

	if(CharacterAttributes.Contains(UpdatedPlayerID))
	{
		//获取待更新角色属性
		const FCharacterAttribute& CharacterAttribute = CharacterAttributes[UpdatedPlayerID];

		//调用本地玩家的PlayerState，更新待更新角色的属性
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
	//TODO:金币奖励和金币UI
	
	if(KillerPlayerID == KilledPlayerID) return;

	//击杀者和被击杀者的PlayerState，如果为nullptr则代表是AI
	AMobaPlayerState* KillerPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), KillerPlayerID);
	AMobaPlayerState* KilledPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), KilledPlayerID);

	const FCharacterAsset* KillerCharacterAsset = GetCharacterAssetFromPlayerID(KillerPlayerID);
	const FCharacterAsset* KilledCharacterAsset = GetCharacterAssetFromPlayerID(KilledPlayerID);
	
	/** 1 击杀者是玩家, 被击杀者是玩家 */
	if(KillerPlayerState && KilledPlayerState)
	{
		//团队击杀
		AddTeamKillCount(KillerPlayerState->GetPlayerDataComponent()->TeamType, 1);
		
		//击杀提示
		MobaKillSystemComponent->KillPrompt(KillerPlayerID, KilledPlayerID);
		
		//击杀结算
		KillerPlayerState->GetPlayerDataComponent()->KillNum++;
		KillerPlayerState->UpdateKDAInfo();

		//被击杀结算
		KilledPlayerState->GetPlayerDataComponent()->DeathNum++;
		KilledPlayerState->UpdateKDAInfo();

		//助攻结算
		for(const auto& Assit : KilledPlayerState->GetAssistPlayers())
		{
			if(Assit.PlayerID != KillerPlayerID)
			{
				if(AMobaPlayerState* AssitPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), Assit.PlayerID))
				{
					AssitPlayerState->GetPlayerDataComponent()->AssistNum++;
					AssitPlayerState->UpdateKDAInfo();
				}
			}
		}
	}
	/** 2 击杀者是玩家, 被击杀者是AI */
	else if(KillerPlayerState && !KilledPlayerState)
	{
		//如果被击杀者是炮塔，则奖励击杀塔的玩家，范围内的队友也获得奖励
		//找到最近助攻的玩家记录击杀数（记为玩家击杀AI）
		if(KilledCharacterAsset->CharacterType >= ECharacterType::ECT_1st_Tower && KilledCharacterAsset->CharacterType <= ECharacterType::ECT_Base_Tower)
		{
			if(AMobaTurretCharacter* Turret = Cast<AMobaTurretCharacter>(MethodUnit::GetMobaCharacterFromPlayerID(GetWorld(),KilledPlayerID)))
			{
				//炮塔击杀者
				if(const FAssistPlayer* LastAssistPlayer = Turret->GetLastAssistPlayer())
				{
					
					//炮塔助攻者
					for(const auto& OtherAssitPlayer : Turret->GetAssistPlayers())
					{
						if(OtherAssitPlayer != *LastAssistPlayer)
						{
							//找到助攻玩家记录击杀数（记为玩家助攻AI）
							if(AMobaPlayerState* AssistPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), OtherAssitPlayer.PlayerID))
							{
							
							}
						}
					}
				}
			}
			if(KillerCharacterAsset && KilledCharacterAsset)
			{
				//收集和更新击杀信息
				FKillNetPackgae KillNetPackgae;
				KillNetPackgae.KillerName = KillerPlayerState->GetPlayerDataComponent()->PlayerName;
				KillNetPackgae.KillerIcon = KillerCharacterAsset->CharacterIcon;
				KillNetPackgae.KilledName = KilledCharacterAsset->CharacterName;
				KillNetPackgae.KilledIcon = KilledCharacterAsset->CharacterIcon;
				UpdateKillMessage(KillNetPackgae);
			}
		}
		//如果击杀小兵和野怪
		else 
		{
			//击杀结算
			KillerPlayerState->GetPlayerDataComponent()->MinionKillNum++;
			KillerPlayerState->UpdateKDAInfo();
		}
	}
	/** 3 击杀者是AI, 被击杀者是玩家 */
	else if(!KillerPlayerState && KilledPlayerState)
	{
		//在一定时间有玩家助攻
		if(const FAssistPlayer* LastAssist = KilledPlayerState->GetLastAssistPlayer())
		{
			//找到最近助攻的玩家记录击杀数（记为玩家击杀玩家）
			if(AMobaPlayerState* LastAssitPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), LastAssist->PlayerID))
			{
				//团队击杀
				AddTeamKillCount(LastAssitPlayerState->GetPlayerDataComponent()->TeamType, 1);

				//击杀提示
				MobaKillSystemComponent->KillPrompt(LastAssist->PlayerID, KilledPlayerID);

				//击杀者结算
				LastAssitPlayerState->GetPlayerDataComponent()->KillNum++;
				LastAssitPlayerState->UpdateKDAInfo();
			
				//被击杀者结算
				KilledPlayerState->GetPlayerDataComponent()->DeathNum++;
				KilledPlayerState->UpdateKDAInfo();
			
				//助攻结算
				for(const auto& Assit : KilledPlayerState->GetAssistPlayers())
				{
					if(Assit != *LastAssist) //排除击杀者
					{
						if(AMobaPlayerState* OtherAssitPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), Assit.PlayerID))
						{
							OtherAssitPlayerState->GetPlayerDataComponent()->AssistNum++;
							OtherAssitPlayerState->UpdateKDAInfo();
						}
					}
				}
			}
		}
		//没有玩家助攻，则播放AI击杀玩家信息
		else
		{
			if(KillerCharacterAsset && KilledCharacterAsset)
			{
				//根据被击杀者队伍，为击杀者队伍增加击杀数
				if(KilledPlayerState->GetPlayerDataComponent()->TeamType == ETeamType::ETT_Blue)
				{
					AddTeamKillCount(ETeamType::ETT_Red, 1);
				}
				else if(KilledPlayerState->GetPlayerDataComponent()->TeamType == ETeamType::ETT_Red)
				{
					AddTeamKillCount(ETeamType::ETT_Blue, 1);
				}
		
				//被击杀者结算
				KilledPlayerState->GetPlayerDataComponent()->DeathNum++;
				KilledPlayerState->UpdateKDAInfo();
		
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
	/** 4 击杀者是AI, 被击杀者是AI */
	else if(!KillerPlayerState && !KilledPlayerState)
	{
		if(KillerCharacterAsset && KilledCharacterAsset)
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

bool AMobaGameState::IsPlayer(int64 PlayerID) const
{
	return MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), PlayerID) != nullptr;
}

void AMobaGameState::Death(int64 PlayerID)
{
	MobaKillSystemComponent->Death(PlayerID);
	
	//清空助攻列表
	if(AMobaPlayerState* MobaPlayState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), PlayerID))
	{
		MobaPlayState->GetMobaAssistSystemComponent()->Death();
	}
}

void AMobaGameState::BindKillFuntion()
{
	//TODO: 可以对三种击杀方式做了区分，这里暂时不区分，统一调用MulticastKillMessage
	
	MobaKillSystemComponent->NormalKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_NormalKill, KillerPlayerID, KilledPlayerID);
	};
	
	MobaKillSystemComponent->FirstBloodFuntion = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_FirstBlood, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->DoubleKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_DoubleKill, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->TripleKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_TripleKill, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->QuadraKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_QuadraKill, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->PentaKillFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_PentaKill, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->DaShaTeShaFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_DaShaTeSha, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->SuoXiangPiMiFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_SuoXiangPiMi, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->HunShenShiDanFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_HunShenShiDan, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->YongGuanSanJunFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_YongGuanSanJun, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->YiJiDangQianFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_YiJiDangQian, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->WanFuMoDiFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_WanFuMoDi, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->JuShiWuShuangFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_JuShiWuShuang, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->TianXiaWuDiFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		MulticastKillMessage(EKillType::EKT_TianXiaWuDi, KillerPlayerID, KilledPlayerID);
	};

	MobaKillSystemComponent->TeamDeathFunction = [&](const int64& KillerPlayerID, const int64& KilledPlayerID)
	{
		bool bTeamDie = true;

		//这里要处理队伍信息
		
		if(AMobaPlayerState* KillPlayerState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(),KillerPlayerID))
		{
			MethodUnit::ServerCallAllMobaHero<AMobaHeroCharacter>(GetWorld(), [&](AMobaHeroCharacter* MobaCharacter)
			{
				//有一个活的，就不算团灭
				if(!MobaCharacter->IsDead())
				{
					bTeamDie = false;
				}
				return MethodUnit::EServerCallType::ECT_InProgress;
			});

			//团灭
			if(bTeamDie)
			{
				GThread::GetCoroutines().BindLambda(3.0f, [&]()
				{
					MulticastKillMessage(EKillType::EKT_TeamDeath, KillerPlayerID, KilledPlayerID);
				});
			}
		}
		
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

void AMobaGameState::AddTeamKillCount(const ETeamType TeamType, const int32 KillCount)
{
	if(TeamType == ETeamType::ETT_Red || TeamType == ETeamType::ETT_Blue)
	{
		TeamKillCount[TeamType] += KillCount;

		MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(),[&](AMobaPlayerState* MobaPlayerState)-> MethodUnit::EServerCallType
		{
			int32 FriendlyKillCount = 0;
			int32 EnemyKillCount = 0;
			if(MobaPlayerState->GetPlayerDataComponent()->TeamType == ETeamType::ETT_Red)
			{
				FriendlyKillCount = TeamKillCount[ETeamType::ETT_Red];
				EnemyKillCount = TeamKillCount[ETeamType::ETT_Blue];
			}
			else if(MobaPlayerState->GetPlayerDataComponent()->TeamType == ETeamType::ETT_Blue)
			{
				FriendlyKillCount = TeamKillCount[ETeamType::ETT_Blue];
				EnemyKillCount = TeamKillCount[ETeamType::ETT_Red];
			}

			MobaPlayerState->Client_UpdateTeamKillCount(FriendlyKillCount, EnemyKillCount);

			return MethodUnit::EServerCallType::ECT_InProgress;
		});
	}
}

