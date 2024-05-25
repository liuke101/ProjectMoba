#include "Character/Hero/MobaHero.h"

#include "Character/MobaPawn.h"
#include "Common/MethodUnit.h"
#include "Component/PlayerDataComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/MobaPlayerState.h"
#include "Table/CharacterAsset.h"

#include "Game/MobaGameState.h"
#include "UI/StatusBar/MobaStatusBarUI.h"


AMobaHero::AMobaHero()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMobaHero::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMobaHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobaHero::InitCharacter()
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

void AMobaHero::MultiCastStatusBar_PlayerName_Implementation(const FString& PlayerName)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		if(const UMobaStatusBarUI* StatusBarUI = Cast<UMobaStatusBarUI>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetName(PlayerName); //设置玩家名字
		}
	}
}

void AMobaHero::MultiCastStatusBar_Level_Implementation(const int32 Level)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		if(const UMobaStatusBarUI* StatusBarUI = Cast<UMobaStatusBarUI>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetLevel(Level); //设置玩家名字
		}
	}
}

void AMobaHero::SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaHero> InTarget) 
{
	if(UAnimMontage* Montage = GetCurrentSkillMontage(SkillKey))
	{
		//广播动画
		MultiCastPlayerAnimMontage(Montage);
	}
}

UAnimMontage* AMobaHero::GetCurrentSkillMontage(ESkillKey SkillKey) const
{
	if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), GetPlayerID()))
	{
		switch (SkillKey)
		{
		case ESkillKey::ESK_W:
			return CharacterAsset->W_SkillMontage;
		case ESkillKey::ESK_E:
			return CharacterAsset->E_SkillMontage;
		case ESkillKey::ESK_R:
			return CharacterAsset->R_SkillMontage;
		case ESkillKey::ESK_F:
			return CharacterAsset->F_SkillMontage;
		case ESkillKey::ESK_Space:
			return CharacterAsset->Space_SkillMontage;
		}
	}
	return nullptr;
}
