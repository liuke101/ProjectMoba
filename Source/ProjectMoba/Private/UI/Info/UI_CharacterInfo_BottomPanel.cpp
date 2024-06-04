// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Info/UI_CharacterInfo_BottomPanel.h"

#include "ThreadManage.h"
#include "Component/PlayerDataComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Game/MobaGameState.h"
#include "Game/MobaPlayerState.h"
#include "ProjectMoba/MobaType.h"
#include "UI/Info/UI_CharacterInfo.h"


void UUI_CharacterInfo_BottomPanel::NativeConstruct()
{
	Super::NativeConstruct();
	
	//绑定 属性更新委托
	GThread::GetCoroutines().BindLambda(0.3f, [&]()
	{
		if(AMobaGameState* MobaGameState = GetMobaGameState())
		{
			MobaGameState->OnUpdateAllAttributesDelegate.AddUObject(this, &UUI_CharacterInfo_BottomPanel::ResponseUpdateSlots);
		}
	});
}

void UUI_CharacterInfo_BottomPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		if(MobaPlayerState->GetPlayerDataComponent())
		{
			// 更新金币
			GolbText->SetText(FText::FromString(FString::FromInt(MobaPlayerState->GetPlayerDataComponent()->Gold)));
		}
	}
}

void UUI_CharacterInfo_BottomPanel::ResponseUpdateSlot(int64 InPlayerID,
                                                       const ECharacterAttributeType CharacterAttributeType)
{
	if(InPlayerID != GetPlayerID()) return;

	//根据PlayerID读取CharacterAttribute，更新UI
	if(AMobaGameState* MobaGameState = GetMobaGameState())
	{
		if(FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(InPlayerID))
		{
			//枚举值看做协议，根据传来的协议值更新UI
			switch (CharacterAttributeType)
			{
			case ECharacterAttributeType::ECAT_Level:
				Level->SetText(FText::FromString(FString::FromInt(CharacterAttribute->Level)));
				break;
			case ECharacterAttributeType::ECAT_MaxHealth:
			case ECharacterAttributeType::ECAT_CurrentHealth:
				//123/500
				HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),static_cast<int32>(CharacterAttribute->CurrentHealth), static_cast<int32>(CharacterAttribute->MaxHealth))));
				HealthBar->SetPercent(CharacterAttribute->GetHealthPercent());
				break;
			case ECharacterAttributeType::ECAT_MaxMana:
			case ECharacterAttributeType::ECAT_CurrentMana:
				ManaText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),static_cast<int32>(CharacterAttribute->CurrentMana), static_cast<int32>(CharacterAttribute->MaxMana))));
				ManaBar->SetPercent(CharacterAttribute->GetManaPercent());
				break;
			case ECharacterAttributeType::ECAT_MaxEXP:
			case ECharacterAttributeType::ECAT_CurrentEXP:
				ExpText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),static_cast<int32>(CharacterAttribute->CurrentExp), static_cast<int32>(CharacterAttribute->MaxExp))));
				ExpBar->SetPercent(CharacterAttribute->GetExpPercent());
				break;
			default:
				break;
			}
		}
	}
}


void UUI_CharacterInfo_BottomPanel::ResponseUpdateSlots(int64 InPlayerID)
{
	if(GetPlayerID() == INDEX_NONE)
	{
		SetPlayerID(InPlayerID);
	}
	
	if(InPlayerID == GetPlayerID())
	{
		//根据PlayerID读取CharacterAttribute，更新UI
		if(AMobaGameState* MobaGameState = GetMobaGameState())
		{
			if(FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(InPlayerID))
			{
				Level->SetText(FText::FromString(FString::FromInt(CharacterAttribute->Level)));
			
				HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),static_cast<int32>(CharacterAttribute->CurrentHealth), static_cast<int32>(CharacterAttribute->MaxHealth))));
				HealthBar->SetPercent(CharacterAttribute->GetHealthPercent());
		
				ManaText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),static_cast<int32>(CharacterAttribute->CurrentMana), static_cast<int32>(CharacterAttribute->MaxMana))));
				ManaBar->SetPercent(CharacterAttribute->GetManaPercent());
			
				ExpText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),static_cast<int32>(CharacterAttribute->CurrentExp), static_cast<int32>(CharacterAttribute->MaxExp))));
				ExpBar->SetPercent(CharacterAttribute->GetExpPercent());

				//初始化CharacterInfo
				CharacterInfo->ResponseUpdateSlots(InPlayerID);
			}
		}
	}
}