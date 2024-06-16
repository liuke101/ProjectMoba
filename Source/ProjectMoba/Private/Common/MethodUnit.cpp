// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MethodUnit.h"

#include "NiagaraRibbonRendererProperties.h"
#include "Character/MobaCharacter.h"
#include "Component/PlayerDataComponent.h"
#include "Game/MobaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Table/SlotAsset.h"
#include "UI/Tip/UI_Tip.h"

AMobaGameState* MethodUnit::GetMobaGameState(const UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetGameState<AMobaGameState>();
	}
	return nullptr;
}

AMobaPlayerState* MethodUnit::GetMobaPlayerStateFromPlayerID(UWorld* InWorld, const int64 PlayerID)
{
	if(InWorld)
	{
		AMobaPlayerState* MobaPlayerState = nullptr;
		ServerCallAllPlayerState<AMobaPlayerState>(InWorld, [&](AMobaPlayerState* PlayerState)
		{
			if(PlayerState->GetPlayerID() == PlayerID)
			{
				MobaPlayerState = PlayerState;
				return EServerCallType::ECT_ProgressComplete;
			}
			return EServerCallType::ECT_InProgress;
		});

		return MobaPlayerState;
	}
	return nullptr;
	
}

AMobaPlayerState* MethodUnit::GetMobaPlayerState(UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetFirstPlayerController()->GetPlayerState<AMobaPlayerState>();
	}
	return nullptr;
}

const FCharacterAsset* MethodUnit::GetCharacterAssetFromCharacterID(const UWorld* InWorld, const int32 CharacterID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAssetFromCharacterID(CharacterID);
	}
	return nullptr;
}

const FCharacterAsset* MethodUnit::GetCharacterAssetFromPlayerID(const UWorld* InWorld, int64 PlayerID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAssetFromPlayerID(PlayerID);
	}
	return nullptr;
}

const FCharacterAttribute* MethodUnit::GetCharacterAttributeFromCharacterID(const UWorld* InWorld, const int32 CharacterID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAttributeFromCharacterID(CharacterID);
	}
	return nullptr;
}


FCharacterAttribute* MethodUnit::GetCharacterAttributeFromPlayerID(const UWorld* InWorld, int64 PlayerID)
{
	if(AMobaGameState* MobaGameState = GetMobaGameState(InWorld))
	{
		return MobaGameState->GetCharacterAttributeFromPlayerID(PlayerID);
	}
	return nullptr;
}


const FSlotAttribute* MethodUnit::GetSlotAttributeFromAnimMontage(AMobaPlayerState* PlayerState,
                                                            const UAnimMontage* AnimMontage)
{
	int32 SkillDataID = INDEX_NONE;
	
	if(PlayerState)
	{
		//根据AnimMontage获取技能的DataID
		if(AMobaGameState* MobaGameState = GetMobaGameState(PlayerState->GetWorld()))
		{
			SkillDataID = MobaGameState->GetSkillDataIDFroSkillMontage(AnimMontage);
		}

		//根据DataID获取SlotAttribute
		if(SkillDataID!=INDEX_NONE)
		{
			return PlayerState->GetSlotAttributeFromDataID(SkillDataID);
		}
	}

	return nullptr;
}

FString MethodUnit::SlotAttributeValueToString(const FSlotAttributeValue* SlotAttributeValue, const FString& AttrbuteName, const FString& ColorTag)
{
	FString Str = "";
	
	if(SlotAttributeValue->Value != 0.0f)
	{
		if(SlotAttributeValue->GainType == ESlotAttributeGainType::ESAGT_Add)
		{
			if(SlotAttributeValue->ValueType == ESlotAttributeValueType::ESAVT_Value)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(FStringFormatArg(AttrbuteName));  //0
				Args.Add(FStringFormatArg(ColorTag));  //1
				Args.Add(FStringFormatArg(static_cast<int32>(SlotAttributeValue->Value)));  //2
 
				Str = FString::Format(TEXT("{0} 属性值 <{1}>+{2}</>\n"), Args);
			}
			else if(SlotAttributeValue->ValueType == ESlotAttributeValueType::ESAVT_Percent)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(FStringFormatArg(AttrbuteName)); 
				Args.Add(FStringFormatArg(ColorTag)); 
				Args.Add(FStringFormatArg(static_cast<int32>(SlotAttributeValue->Value*100.0f))); 
				
				Str = FString::Format(TEXT("{0} 属性百分比 <{1}>+{2}%</>\n"), Args);
			}
		}
		else if(SlotAttributeValue->GainType == ESlotAttributeGainType::ESAGT_Substract)
		{
			if(SlotAttributeValue->ValueType == ESlotAttributeValueType::ESAVT_Value)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(FStringFormatArg(AttrbuteName));  //0
				Args.Add(FStringFormatArg(ColorTag));  //1
				Args.Add(FStringFormatArg(static_cast<int32>(SlotAttributeValue->Value)));  //2
 
				Str = FString::Format(TEXT("{0} 属性值 <{1}>-{2}</>\n"), Args);
			}
			else if(SlotAttributeValue->ValueType == ESlotAttributeValueType::ESAVT_Percent)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(FStringFormatArg(AttrbuteName)); 
				Args.Add(FStringFormatArg(ColorTag)); 
				Args.Add(FStringFormatArg(static_cast<int32>(SlotAttributeValue->Value*100.0f))); 
				
				Str = FString::Format(TEXT("{0} 属性百分比 <{1}>-{2}%</>\n"), Args);
			}
		}
	}
	return Str;
}

FText MethodUnit::GetAttributeDescription(const FSlotAttribute* SlotAttribute)
{
	if(SlotAttribute)
	{
		FString CurrentHealth = SlotAttributeValueToString(&SlotAttribute->CurrentHealth, TEXT("当前生命值"), TEXT("Red"));
		FString MaxHealth = SlotAttributeValueToString(&SlotAttribute->MaxHealth, TEXT("最大生命值"), TEXT("Red"));
		FString CurrentMana = SlotAttributeValueToString(&SlotAttribute->CurrentMana, TEXT("当前魔法值"), TEXT("Blue"));
		FString MaxMana = SlotAttributeValueToString(&SlotAttribute->MaxMana, TEXT("最大魔法值"), TEXT("Blue"));
		FString PhysicalAttack = SlotAttributeValueToString(&SlotAttribute->PhysicalAttack, TEXT("物理攻击"), TEXT("Yellow"));
		FString Armor = SlotAttributeValueToString(&SlotAttribute->Armor, TEXT("护甲"), TEXT("Yellow"));
		FString PhysicalPenetration = SlotAttributeValueToString(&SlotAttribute->PhysicalPenetration, TEXT("物理穿透"), TEXT("Yellow"));
		FString MagicAttack = SlotAttributeValueToString(&SlotAttribute->MagicAttack, TEXT("魔法攻击"), TEXT("Purple"));
		FString MagicResistance = SlotAttributeValueToString(&SlotAttribute->MagicResistance, TEXT("魔抗"), TEXT("Purple"));
		FString MagicPenetration = SlotAttributeValueToString(&SlotAttribute->MagicPenetration, TEXT("魔法穿透"), TEXT("Purple"));
		FString WalkSpeed = SlotAttributeValueToString(&SlotAttribute->WalkSpeed, TEXT("移动速度"), TEXT("Green"));
		FString AttackSpeed = SlotAttributeValueToString(&SlotAttribute->AttackSpeed, TEXT("攻击速度"), TEXT("Green"));
		FString CriticalRate = SlotAttributeValueToString(&SlotAttribute->CriticalRate, TEXT("暴击率"), TEXT("Orange"));
		
		//合并所有非空字符串
		FString Str = CurrentHealth + MaxHealth + CurrentMana + MaxMana + PhysicalAttack + Armor + PhysicalPenetration + MagicAttack + MagicResistance + MagicPenetration + WalkSpeed + AttackSpeed + CriticalRate;
		return FText::FromString(Str);
	}
	
	return FText::FromString(TEXT(""));
}


void MethodUnit::SetToolTip(UUI_Tip* Tip, const FSlotAttribute* SlotAttribute, const FSlotAsset* SlotAsset)
{
	if(Tip)
	{
		if(SlotAsset)
		{
			//名称  描述
			Tip->SetTextName(FText::FromName(SlotAsset->SlotName));
			Tip->SetTextDescription(SlotAsset->SlotDescription);
		}

		if(SlotAttribute)
		{
			//获取基础属性描述
			FText BaseAttribute = MethodUnit::GetAttributeDescription(SlotAttribute);
			Tip->SetRichTextAction(BaseAttribute);
			//获取主动技能描述
			FText ActiveAttribute = MethodUnit::GetAttributeDescription(SlotAttribute->ActiveSkillAttribute);
			Tip->SetRichTextActive(ActiveAttribute);
			//获取被动技能描述
			FText PassiveAttribute = MethodUnit::GetAttributeDescription( SlotAttribute->BuffAttribute);
			Tip->SetRichTextPassive(PassiveAttribute);
		}
	}
}

void MethodUnit::RegisterSlotAttribute(AMobaPlayerState* PlayerState, FSlotAttribute* SlotAttribute)
{
	//addlevel
	if(SlotAttribute->AddLevelDataID != INDEX_NONE)
	{
		if(const FSlotAttribute* AddLevelAttribute = PlayerState->GetSlotAttributeFromDataID(SlotAttribute->AddLevelDataID))
		{
			SlotAttribute->AddLevelAttribute = AddLevelAttribute;
		}
	}

	//buff 被动
	if(SlotAttribute->BuffDataID != INDEX_NONE)
	{
		if(const FSlotAttribute* BuffAttribute = PlayerState->GetSlotAttributeFromDataID(SlotAttribute->BuffDataID))
		{
			SlotAttribute->BuffAttribute = BuffAttribute;
		}
	}

	//主动
	if(SlotAttribute->ActiveSkillDataID != INDEX_NONE)
	{
		if(const FSlotAttribute* ActiveSkillAttribute = PlayerState->GetSlotAttributeFromDataID(SlotAttribute->ActiveSkillDataID))
		{
			SlotAttribute->ActiveSkillAttribute = ActiveSkillAttribute;
		}
	}
}

AMobaPawn* MethodUnit::GetMobaPawnFromPlayerID(UWorld* InWorld, int64 PlayerID)
{
	AMobaPawn* MobaPawn = nullptr;
	ServerCallAllPlayer<AMobaPawn>(InWorld, [&](AMobaPawn* InMobaPawn)
	{
		if(InMobaPawn->GetPlayerID() == PlayerID)
		{
			MobaPawn = InMobaPawn;
			return EServerCallType::ECT_ProgressComplete;
		}
		return EServerCallType::ECT_InProgress;
	});

	return MobaPawn;
}

AMobaPawn* MethodUnit::GetMobaPawn(UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetFirstPlayerController()->GetPawn<AMobaPawn>();
	}
	return nullptr;
}


AMobaCharacter* MethodUnit::GetMobaCharacterFromPlayerID(UWorld* InWorld, int64 PlayerID)
{
	TArray<AActor*> FindActors{};
	UGameplayStatics::GetAllActorsOfClass(InWorld, AMobaCharacter::StaticClass(), FindActors);
	for(AActor* Actor : FindActors)
	{
		if(AMobaCharacter* MobaCharacter = Cast<AMobaCharacter>(Actor))
		{
			if(MobaCharacter->GetPlayerID() == PlayerID)
			{
				return MobaCharacter;
			}
		}
	}
	return nullptr;
}

bool MethodUnit::IsPlayer(UWorld* InWorld, int64 InPlayerID)
{
	ServerCallAllPlayerDataComponent<UPlayerDataComponent>(InWorld, [&](const UPlayerDataComponent* DataComponent)
	{
		if(DataComponent->PlayerID == InPlayerID)
		{
			return EServerCallType::ECT_ProgressComplete; //终止查找
		}
		return EServerCallType::ECT_InProgress; //继续查找
	});
	
	return false;
}

bool MethodUnit::IsFriendly(const AMobaCharacter* CharacterA, const AMobaCharacter* CharacterB)
{
	if(CharacterA && CharacterB)
	{
		return CharacterA->GetTeamType() == CharacterB->GetTeamType();
	}
	return true;
}




