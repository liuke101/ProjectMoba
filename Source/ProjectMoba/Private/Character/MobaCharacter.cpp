// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaCharacter.h"

#include "Common/MethodUnit.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Game/MobaGameState.h"
#include "Table/CharacterAsset.h"
#include "UI/StatusBar/MobaStatusBarUI.h"

AMobaCharacter::AMobaCharacter()
	: bAttacking(false),
	  AttackCount(0),
	  PlayerID(INDEX_NONE)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
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

	//开火点
	FirePointComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePointComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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
		if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), PlayerID))
		{
			if(AttackCount<CharacterAsset->NormalAttackMontages.Num())
			{
				if(UAnimMontage* Montage = CharacterAsset->NormalAttackMontages[AttackCount])
				{
					if(AttackCount == CharacterAsset->NormalAttackMontages.Num()-1)
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
	if(UAnimMontage* Montage = GetCurrentSkillMontage(SkillKey))
	{
		//广播动画
		MultiCastPlayerAnimMontage(Montage);
	}
}

UAnimMontage* AMobaCharacter::GetCurrentSkillMontage(ESkillKey SkillKey) const
{
	if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), PlayerID))
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

void AMobaCharacter::MultiCastStatusBar_Health_Implementation(float HealthPercent)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		//更新HealthBar
		if(UMobaStatusBarUI* StatusBarUI = Cast<UMobaStatusBarUI>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetHealthPercent(HealthPercent);
		}
	}
}

void AMobaCharacter::MultiCastStatusBar_Mana_Implementation(float ManaPercent)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		//更新ManaBar
		if(UMobaStatusBarUI* StatusBarUI = Cast<UMobaStatusBarUI>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetManaPercent(ManaPercent);
		}
	}
}

void AMobaCharacter::MultiCastReborn_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		GetCharacterAttribute()->ResetAttribute();
	}
	StopAnimMontage(); //停止死亡动画
}

void AMobaCharacter::RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID, const ETeamType InTeamType)
{
	SetPlayerID(InPlayerID);
	SetTeamType(InTeamType);
	
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
			MultiCastStatusBar_Health(Attribute->GetHealthPercent());
			MultiCastStatusBar_Mana(Attribute->GetManaPercent());
		}
	}
}

FCharacterAttribute* AMobaCharacter::GetCharacterAttribute() 
{
	return MethodUnit::GetCharacterAttributeFromPlayerID(GetWorld(), PlayerID);
}

bool AMobaCharacter::IsDead()
{
	if(GetCharacterAttribute()->CurrentHealth <= 0.0f)
	{
		return true;
	}
	return false;
}

FVector AMobaCharacter::GetFirePointLocation() const
{
	return FirePointComponent->GetComponentLocation();
}

FRotator AMobaCharacter::GetFirePointRotation() const
{
	return FirePointComponent->GetComponentRotation();
}

float AMobaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		GetCharacterAttribute()->CurrentHealth += DamageAmount;
		MultiCastStatusBar_Health(GetCharacterAttribute()->GetHealthPercent());
		
		//攻击角色
		if(IsDead())
		{
			//死亡动画广播到客户端
			MultiCastPlayerAnimMontage(MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), PlayerID)->DeathMontage);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("造成伤害：") + FString::SanitizeFloat(Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser)));
		}
	}
	return 0.0f;
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

