// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MethodUnit.h"

#include "NiagaraRibbonRendererProperties.h"
#include "Character/MobaCharacter.h"
#include "Component/PlayerDataComponent.h"
#include "Game/MobaGameState.h"
#include "Kismet/GameplayStatics.h"

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
			SkillDataID = MobaGameState->GetSkillDataIDFromAnimMontage(AnimMontage);
		}

		//根据DataID获取SlotAttribute
		if(SkillDataID!=INDEX_NONE)
		{
			return PlayerState->GetSlotAttributeFromDataID(SkillDataID);
		}
	}

	return nullptr;
}

FText MethodUnit::SlotAttributeValueToText(const FSlotAttributeValue* SlotAttributeValue, const FString& ColorTag, const FString& Description)
{
	if(SlotAttributeValue->Value != 0.0f)
	{
		if(SlotAttributeValue->GainType == ESlotAttributeGainType::ESAGT_Add)
		{
			if(SlotAttributeValue->ValueType == ESlotAttributeValueType::ESAVT_Value)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(FStringFormatArg(ColorTag)); //0
				Args.Add(FStringFormatArg(static_cast<int32>(SlotAttributeValue->Value))); //1
				Args.Add(FStringFormatArg(Description)); //2
 
				FString Str = FString::Format(TEXT("值增加, <{0}>+{1}</> {2}"), Args);
				
				return FText::FromString(Str);
			}
			else if(SlotAttributeValue->ValueType == ESlotAttributeValueType::ESAVT_Percent)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(FStringFormatArg(ColorTag)); //0
				Args.Add(FStringFormatArg(static_cast<int32>(SlotAttributeValue->Value*100.0f))); //1
				Args.Add(FStringFormatArg(Description)); //2
 
				FString Str = FString::Format(TEXT("百分比增加, <{0}>+{1}%</> {2}"), Args);
				return FText::FromString(Str);
			}
		}
		else if(SlotAttributeValue->GainType == ESlotAttributeGainType::ESAGT_Substract)
		{
			if(SlotAttributeValue->ValueType == ESlotAttributeValueType::ESAVT_Value)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(FStringFormatArg(ColorTag)); //0
				Args.Add(FStringFormatArg(static_cast<int32>(SlotAttributeValue->Value))); //1
				Args.Add(FStringFormatArg(Description)); //2
 
				FString Str = FString::Format(TEXT("值减少, <{0}>-{1}</> {2}"), Args);
				
				return FText::FromString(Str);
			}
			else if(SlotAttributeValue->ValueType == ESlotAttributeValueType::ESAVT_Percent)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(FStringFormatArg(ColorTag)); //0
				Args.Add(FStringFormatArg(static_cast<int32>(SlotAttributeValue->Value*100.0f))); //1
				Args.Add(FStringFormatArg(Description)); //2
 
				FString Str = FString::Format(TEXT("百分比减少, <{0}>-{1}%</> {2}"), Args);
				return FText::FromString(Str);
			}
		}
	}
	return FText::FromString(TEXT(""));
}

FText MethodUnit::GetBaseAttributeDescription(const FSlotAttribute* SlotAttribute)
{
	if(SlotAttribute)
	{
		FText CurrentHealth = SlotAttributeValueToText(&SlotAttribute->CurrentHealth, TEXT("Red"), TEXT("当前生命值"));
		FText MaxHealth = SlotAttributeValueToText(&SlotAttribute->MaxHealth, TEXT("Red"), TEXT("最大生命值"));
		FText CurrentMana = SlotAttributeValueToText(&SlotAttribute->CurrentMana, TEXT("Blue"), TEXT("当前魔法值"));
		FText MaxMana = SlotAttributeValueToText(&SlotAttribute->MaxMana, TEXT("Blue"), TEXT("最大魔法值"));
		FText PhysicalAttack = SlotAttributeValueToText(&SlotAttribute->PhysicalAttack, TEXT("Yellow"), TEXT("物理攻击"));
		FText Armor = SlotAttributeValueToText(&SlotAttribute->Armor, TEXT("Yellow"), TEXT("护甲"));
		FText PhysicalPenetration = SlotAttributeValueToText(&SlotAttribute->PhysicalPenetration, TEXT("Yellow"), TEXT("物理穿透"));
		FText MagicAttack = SlotAttributeValueToText(&SlotAttribute->MagicAttack, TEXT("Purple"), TEXT("魔法攻击"));
		FText MagicResistance = SlotAttributeValueToText(&SlotAttribute->MagicResistance, TEXT("Purple"), TEXT("魔抗"));
		FText MagicPenetration = SlotAttributeValueToText(&SlotAttribute->MagicPenetration, TEXT("Purple"), TEXT("魔法穿透"));
		FText WalkSpeed = SlotAttributeValueToText(&SlotAttribute->WalkSpeed, TEXT("Green"), TEXT("移动速度"));
		FText AttackSpeed = SlotAttributeValueToText(&SlotAttribute->AttackSpeed, TEXT("Green"), TEXT("攻击速度"));
		FText CriticalRate = SlotAttributeValueToText(&SlotAttribute->CriticalRate, TEXT("Orange"), TEXT("暴击率"));
		
		return FText::Format(CurrentHealth, MaxHealth, CurrentMana, MaxMana, PhysicalAttack, Armor, PhysicalPenetration, MagicAttack, MagicResistance, MagicPenetration, WalkSpeed, AttackSpeed, CriticalRate);
	}
	
	return FText::FromString(TEXT(""));
}

FText MethodUnit::GetAttributeDescription(UWorld* InWorld, int32 DataID)
{
	if(DataID != INDEX_NONE)
	{
		if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState(InWorld))
		{
			if(FSlotAttribute* SlotAttribute = MobaPlayerState->GetSlotAttributeFromDataID(DataID))
			{
				return GetBaseAttributeDescription(SlotAttribute);
			}
		}
	}
	return FText::FromString(TEXT(""));
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




