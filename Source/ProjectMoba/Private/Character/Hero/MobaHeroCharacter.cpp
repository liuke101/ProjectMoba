#include "Character/Hero/MobaHeroCharacter.h"

#include "Character/MobaPawn.h"
#include "Common/MethodUnit.h"
#include "Component/PlayerDataComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/MobaPlayerState.h"
#include "Table/CharacterAsset.h"

#include "Game/MobaGameState.h"
#include "UI/StatusBar/UI_StatusBar.h"


AMobaHeroCharacter::AMobaHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMobaHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMobaHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobaHeroCharacter::InitCharacter()
{
	Super::InitCharacter();

	//服务器同步玩家名字
	 MethodUnit::ServerCallAllPlayer<AMobaPawn>(GetWorld(), [&](AMobaPawn* InPawn)-> MethodUnit::EServerCallType
	 {
	 	if(InPawn->GetPlayerID() == this->PlayerID)
	 	{
	 		if(const AMobaPlayerState* MobaPlayerState = InPawn->GetPlayerState<AMobaPlayerState>())
	 		{
	 			MultiCastStatusBar_PlayerName(MobaPlayerState->GetPlayerDataComponent()->PlayerName.ToString());
	 		}
	 		return MethodUnit::EServerCallType::ECT_ProgressComplete;
	 	}
	 	return MethodUnit::EServerCallType::ECT_InProgress;
	 });

	if(FCharacterAttribute* CharacterAttribute = MethodUnit::GetCharacterAttributeFromPlayerID(GetWorld(), GetPlayerID()))
	{
		MultiCastStatusBar_Level(CharacterAttribute->Level);
	}
}

void AMobaHeroCharacter::MultiCastStatusBar_PlayerName_Implementation(const FString& PlayerName)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		if(const UUI_StatusBar* StatusBarUI = Cast<UUI_StatusBar>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetName(PlayerName); //设置玩家名字
		}
	}
}

void AMobaHeroCharacter::MultiCastStatusBar_Level_Implementation(const int32 Level)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		if(const UUI_StatusBar* StatusBarUI = Cast<UUI_StatusBar>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetLevel(Level); //设置玩家名字
		}
	}
}

void AMobaHeroCharacter::SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaHeroCharacter> InTarget) 
{
	if(UAnimMontage* Montage = GetCurrentSkillMontage(SkillKey))
	{
		//广播动画
		MultiCastPlayerAnimMontage(Montage);
	}
}

UAnimMontage* AMobaHeroCharacter::GetCurrentSkillMontage(ESkillKey SkillKey) const
{
	if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), GetPlayerID()))
	{
		switch (SkillKey)
		{
		case ESkillKey::ESK_W:
			return CharacterAsset->W_Skill.SkillMontage;
		case ESkillKey::ESK_E:
			return CharacterAsset->E_Skill.SkillMontage;
		case ESkillKey::ESK_R:
			return CharacterAsset->R_Skill.SkillMontage;
		case ESkillKey::ESK_F:
			return CharacterAsset->F_Skill.SkillMontage;
		default:
			break;
		}
	}
	return nullptr;
}
