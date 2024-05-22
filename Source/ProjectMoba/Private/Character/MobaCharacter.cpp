// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaCharacter.h"

#include "Common/MethodUnit.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Table/CharacterAsset.h"
#include "UI/ProgressBar/MobaStatusBarUI.h"

AMobaCharacter::AMobaCharacter()
	: bAttacking(false),
	  AttackCount(0),
	  PlayerID(INDEX_NONE)
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//为了使用行为树的Rotate To Face BB Entry节点，旋转属性设置如下
	GetCharacterMovement()->bOrientRotationToMovement = false;    //必须关闭
	GetCharacterMovement()->bUseControllerDesiredRotation = true; //必须开启
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 状态栏
	StatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusBar"));
	StatusBarComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMobaCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	// 在服务器上每帧更新角色位置
	if(GetLocalRole() == ROLE_Authority)
	{
		if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
		{
			MobaGameState->UpdateCharacterLocation(GetPlayerID(), GetActorLocation());
		}
	}
}

void AMobaCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(GetLocalRole()==ROLE_Authority)
	{
		SpawnDefaultController(); //生成AIController
	}
}

void AMobaCharacter::NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget)
{
	if(InTarget.IsValid())
	{
		//TODO:BUG 
		if(const FCharacterAsset* CharacterTable = MethodUnit::GetMobaGameState(GetWorld())->GetCharacterAssetFromCharacterID(PlayerID))
		{
			if(AttackCount<CharacterTable->NormalAttackMontages.Num())
			{
				if(UAnimMontage* Montage = CharacterTable->NormalAttackMontages[AttackCount])
				{
					if(AttackCount == CharacterTable->NormalAttackMontages.Num()-1)
					{
						AttackCount = 0;
					}
					else
					{
						AttackCount++;
					}

					//广播动画
					MultiCastPlayerAnimMontage(Montage);
				}
			}
		}
	}
}

void AMobaCharacter::SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaCharacter> InTarget)
{
	if(const FCharacterAsset* CharacterTable = MethodUnit::GetMobaGameState(GetWorld())->GetCharacterAssetFromCharacterID(PlayerID))
	{
		if(UAnimMontage* Montage = GetCurrentSkillMontage(SkillKey))
		{
			//广播动画
			MultiCastPlayerAnimMontage(Montage);
		}
	}
}

UAnimMontage* AMobaCharacter::GetCurrentSkillMontage(ESkillKey SkillKey) const
{
	if(const FCharacterAsset* CharacterTable = MethodUnit::GetMobaGameState(GetWorld())->GetCharacterAssetFromCharacterID(PlayerID))
	{
		switch (SkillKey)
		{
		case ESkillKey::ESK_W:
			return CharacterTable->W_SkillMontage;
		case ESkillKey::ESK_E:
			return CharacterTable->E_SkillMontage;
		case ESkillKey::ESK_R:
			return CharacterTable->R_SkillMontage;
		case ESkillKey::ESK_F:
			return CharacterTable->F_SkillMontage;
		case ESkillKey::ESK_Space:
			return CharacterTable->Space_SkillMontage;
		}
	}
	return nullptr;
}

void AMobaCharacter::MultiCastStatusBar_Implementation(float HealthPercent, float ManaPercent)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		if(UMobaStatusBarUI* StatusBarUI = Cast<UMobaStatusBarUI>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetHealthPercent(HealthPercent);
			StatusBarUI->SetManaPercent(ManaPercent);
		}
	}
}

void AMobaCharacter::RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID)
{
	SetPlayerID(InPlayerID);
	
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		MobaGameState->Add_PlayerID_To_CharacterAttribute(InPlayerID, InCharacterID);
		MobaGameState->AddCharacterLocation(InPlayerID, GetActorLocation());

		
		// 使用计时器短暂延迟，保证客户端生成角色后再同步状态栏信息
		GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &AMobaCharacter::InitCharacter, 0.5f, false);
	}
}

void AMobaCharacter::InitCharacter()
{
	// 关闭计时器
	if(InitTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(InitTimerHandle);
	}

	// 广播状态栏
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		if(const FCharacterAttribute* Attribute = MobaGameState->GetCharacterAttributeFromPlayerID(PlayerID))
		{
			MultiCastStatusBar(Attribute->GetHealthPercent(), Attribute->GetManaPercent());
		}
	}
}

const FCharacterAttribute* AMobaCharacter::GetCharacterAttribute() const
{
	return MethodUnit::GetCharacterAttributeFromPlayerID(GetWorld(), PlayerID);
}

bool AMobaCharacter::IsDie() const
{
	if(GetCharacterAttribute()->CurrentHealth <= 0.0f)
	{
		return true;
	}
	return false;
}


void AMobaCharacter::MultiCastPlayerAnimMontage_Implementation(UAnimMontage* InAnimMontage, float InPlayRate,
                                                               FName StartSectionName)
{
	if(InAnimMontage)
	{
		PlayAnimMontage(InAnimMontage, InPlayRate, StartSectionName);
		//是否完成播放
	}
}

