#include "Character/Hero/MobaHeroCharacter.h"

#include "ThreadManage.h"
#include "Actor/DrawText.h"
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

	if(GetLocalRole() == ROLE_Authority)
	{
		GThread::GetCoroutines().BindLambda(0.5f, [&]()
		{
			MethodUnit::ServerCallAllMobaCharacter<AMobaCharacter>(GetWorld(), [&](AMobaCharacter* MobaCharacter)
			{
				MobaCharacter->InitWidgetInfo();
				return MethodUnit::EServerCallType::ECT_InProgress;
			});
		});
	}
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


void AMobaHeroCharacter::SkillAttack(int32 SkillDataID)
{
	if(UAnimMontage* Montage = GetSkillMontageFromDataID(SkillDataID))
	{
		//广播动画
		Multicast_PlayerAnimMontage(Montage);
	}
}

UAnimMontage* AMobaHeroCharacter::GetSkillMontageFromDataID(int32 SkillDataID) const
{
	if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), GetPlayerID()))
	{
		if(CharacterAsset->W_Skill.DataID == SkillDataID)
		{
			return CharacterAsset->W_Skill.SkillMontage;
		}

		if (CharacterAsset->E_Skill.DataID == SkillDataID)
		{
			return CharacterAsset->E_Skill.SkillMontage;
		}

		if (CharacterAsset->R_Skill.DataID == SkillDataID)
		{
			return CharacterAsset->R_Skill.SkillMontage;
		}

		if (CharacterAsset->F_Skill.DataID == SkillDataID)
		{
			return CharacterAsset->F_Skill.SkillMontage;
		}
	}
	return nullptr;
}

void AMobaHeroCharacter::AddExp(float InExp)
{
	if(InExp!=0)
	{
		if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
		{
			if(FCharacterAttribute* CharacterAttribute = GetCharacterAttribute())
			{
				CharacterAttribute->CurrentExp += InExp;

				//如果升级
				if(CharacterAttribute->CurrentExp >= CharacterAttribute->MaxExp)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("升级"));
					
					CharacterAttribute->CurrentExp -= CharacterAttribute->MaxExp;
					
					CharacterAttribute->UpdateLevel();

					//技能点增加
					if(AMobaPlayerState* MobaPlayerState = GetPlayerState<AMobaPlayerState>())
					{
						MobaPlayerState->GetPlayerDataComponent()->SkillPoint++;

						//TODO:现实技能加点UI
					}
					
					MobaGameState->RequestUpdateCharacterAttribute( GetPlayerID(),  GetPlayerID(),ECharacterAttributeType::ECAT_All);
					
					Multicast_StatusBar(CharacterAttribute->GetHealthPercent(), CharacterAttribute->GetManaPercent());

					
					//如果升级后还能升级，则递归调用
					if(CharacterAttribute->CurrentExp >= CharacterAttribute->MaxExp)
					{
						AddExp(CharacterAttribute->CurrentExp - CharacterAttribute->MaxExp);
					}
				}
				else
				{
					MobaGameState->RequestUpdateCharacterAttribute( GetPlayerID(),  GetPlayerID(),ECharacterAttributeType::ECAT_CurrentEXP);
				}
			}
		}
	}
}

void AMobaHeroCharacter::Multicast_SpwanDrawGoldText_Implementation(int32 Value, float Percent,
                                                                    const FLinearColor& Color, const FVector& Location)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		if(ADrawText* DrawText = GetWorld()->SpawnActor<ADrawText>(DrawTextClass, Location, FRotator::ZeroRotator))
		{
			DrawText->SetTextBlock(FString::Printf(TEXT("+%d"), Value), Color, Percent,EInfoAnimType::TYPE_SMALLER,0); //ID0 对应SimpleDrawTextStyle中的表格数据
		}
	}
}
