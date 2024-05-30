// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaCharacter.h"

#include "ThreadManage.h"
#include "Common/MethodUnit.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Game/MobaGameState.h"
#include "ProjectMoba/GlobalVariable.h"
#include "Table/CharacterAsset.h"
#include "UI/StatusBar/UI_StatusBar.h"

AMobaCharacter::AMobaCharacter()
	: bAttacking(false),
	  AttackCount(0),
	  PlayerID(INDEX_NONE)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Character,ECR_Block);

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

void AMobaCharacter::MultiCastStatusBar_Implementation(float HealthPercent, float ManaPercent)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		//英雄的状态条
		if(UUI_StatusBar* StatusBarUI = Cast<UUI_StatusBar>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetHealthPercent(HealthPercent);
			StatusBarUI->SetManaPercent(ManaPercent);
		}
		// 怪物的血条
		else if(UUI_StatusBar_Health* StatusBarUI_Health = Cast<UUI_StatusBar_Health>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI_Health->SetHealthPercent(HealthPercent);
		}

		
	}
}

void AMobaCharacter::MultiCastStatusBar_Health_Implementation(float HealthPercent)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		//更新HealthBar
		if(UUI_StatusBar* StatusBarUI = Cast<UUI_StatusBar>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetHealthPercent(HealthPercent);
		}
		// 怪物的血条
		else if(UUI_StatusBar_Health* StatusBarUI_Health = Cast<UUI_StatusBar_Health>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI_Health->SetHealthPercent(HealthPercent);
		}

	}
}

void AMobaCharacter::MultiCastStatusBar_Mana_Implementation(float ManaPercent)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		//更新ManaBar
		if(UUI_StatusBar* StatusBarUI = Cast<UUI_StatusBar>(StatusBarComponent->GetUserWidgetObject()))
		{
			StatusBarUI->SetManaPercent(ManaPercent);
		}
	}
}

void AMobaCharacter::MultiCastReborn_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		if(FCharacterAttribute* CharacterAttribute = GetCharacterAttribute())
		{
			CharacterAttribute->ResetAttribute();
			MultiCastStatusBar(CharacterAttribute->GetHealthPercent(), CharacterAttribute->GetManaPercent());
		}
	}
	StopAnimMontage(); //停止死亡动画
}

void AMobaCharacter::RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID, const ETeamType InTeamType, const ECharacterType InCharacterType)
{
	SetPlayerID(InPlayerID);
	SetTeamType(InTeamType);
	SetCharacterType(InCharacterType);
	
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		MobaGameState->AddCharacterAttribute(InPlayerID, InCharacterID);
		MobaGameState->AddCharacterLocation(InPlayerID, GetActorLocation());
		
		// 短暂延迟，保证客户端生成角色后再同步状态栏信息
		GThread::GetCoroutines().BindUObject(0.5f, this, &AMobaCharacter::InitCharacter);
	}
}

void AMobaCharacter::InitCharacter()
{
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		if(const FCharacterAttribute* Attribute = MobaGameState->GetCharacterAttributeFromPlayerID(PlayerID))
		{
			GetCharacterMovement()->MaxWalkSpeed = Attribute->WalkSpeed;  //设置移动速度
			MultiCastStatusBar(Attribute->GetHealthPercent(), Attribute->GetManaPercent()); // 广播状态栏
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

void AMobaCharacter::ResetSpeed(float InSpeed)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		if(InSpeed == -1)
		{
			GetCharacterMovement()->MaxWalkSpeed = GetCharacterAttribute()->WalkSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = InSpeed;
		}
	}
	
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
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if(GetLocalRole() == ROLE_Authority)
	{
		GetCharacterAttribute()->CurrentHealth += DamageAmount;
		MultiCastStatusBar_Health(GetCharacterAttribute()->GetHealthPercent());
		
		//攻击角色
		if(IsDead())
		{
			//随机播放死亡动画广播到客户端
			if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), PlayerID))
			{
				MultiCastPlayerAnimMontage(CharacterAsset->DeathMontages[FMath::RandRange(0, CharacterAsset->DeathMontages.Num()-1)]);
			}
			
			//5s后复活
			GThread::GetCoroutines().BindUObject(5.0f, this, &AMobaCharacter::MultiCastReborn);
			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("造成伤害：") + FString::SanitizeFloat(Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser)));
		}
	}
	return 0;
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

