// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaCharacter.h"

#include "ThreadManage.h"
#include "Actor/DrawText.h"
#include "Common/MethodUnit.h"
#include "Common/MobaDamageType.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Game/MobaGameState.h"
#include "Item/Bullet.h"
#include "Net/UnrealNetwork.h"
#include "ProjectMoba/GlobalVariable.h"
#include "Table/CharacterAsset.h"
#include "UI/StatusBar/UI_StatusBar.h"

AMobaCharacter::AMobaCharacter()
	: bAttacking(false),
	  AttackCount(0),
	  PlayerID(INDEX_NONE),
	  RebornTime(10.0f),
      bLockRotate(false)
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

	CurrentRotation = GetActorRotation();
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

		//在Tick中锁定旋转
		if(bLockRotate)
		{
			SetActorRotation(CurrentRotation);
		}
	}
}

void AMobaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMobaCharacter, TeamType, COND_SimulatedOnly);
}

void AMobaCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(GetLocalRole()==ROLE_Authority)
	{
		SpawnDefaultController(); //生成AIController
	}
	else
	{
		InitHealthBarColorfromTeamType();
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
					Multicast_PlayerAnimMontage(Montage);
				}
			}
		}
	}
}

bool AMobaCharacter::IsDead()
{
	if(FCharacterAttribute* CharacterAttribute = GetCharacterAttribute())
	{
		if(CharacterAttribute->CurrentHealth <= 0.0f)
		{
			return true;
		}
	}
	return false;
}

float AMobaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if(GetLocalRole() == ROLE_Authority)
	{
		if(!IsDead())
		{
			if(AMobaCharacter* InDamageCauser = Cast<AMobaCharacter>(DamageCauser))
			{
				//友军检测
				if(MethodUnit::IsFriendly(InDamageCauser, this)) return 0;
				
				if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
				{
					if(FCharacterAttribute* CharacterAttribute = GetCharacterAttribute())
					{
						if(UMobaDamageType* MobaDamageType = DamageEvent.DamageTypeClass->GetDefaultObject<UMobaDamageType>())
						{
							const FSlotAttribute* SlotAttribute = MobaDamageType->SlotAttribute;

							//添加属性到角色
							if(SlotAttribute)
							{
								if(SlotAttribute->AttributeType == ESlotAttributeType::ESAT_Continuous)
								{
									if(AMobaPlayerState* MobaPlayState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(),PlayerID))
									{
										int32 SlotID = FMath::RandRange(0, 9999999); //随机
										MobaPlayState->AddSlotAttributes(SlotID, SlotAttribute);
									}
								}
							}

							//计算血量
							CharacterAttribute->CurrentHealth += DamageAmount;
							if(CharacterAttribute->CurrentHealth > CharacterAttribute->GetMaxHealth())
							{
								CharacterAttribute->CurrentHealth = CharacterAttribute->GetMaxHealth();
							}
							else if(CharacterAttribute->CurrentHealth <= 0.0f)
							{
								CharacterAttribute->CurrentHealth = 0.0f;
							}
	
							//更新UI
							Multicast_StatusBar_Health(CharacterAttribute->GetHealthPercent()); //血条
							MobaGameState->RequestUpdateCharacterAttribute(PlayerID, PlayerID,ECharacterAttributeType::ECAT_CurrentHealth);//属性面板
			
							//伤害字体
							Multicast_SpwanDrawText(DamageAmount, FMath::Abs(DamageAmount)/CharacterAttribute->GetMaxHealth(), FColor::White, GetActorLocation());
			
							//伤害
							if(IsDead()) //死亡
							{
								//随机播放死亡动画广播到客户端
								if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), PlayerID))
								{
									Multicast_PlayerAnimMontage(CharacterAsset->DeathMontages[FMath::RandRange(0, CharacterAsset->DeathMontages.Num()-1)]);
								}

								//死亡结算
								if(MobaGameState->IsPlayer(PlayerID))
								{
									MobaGameState->Death(PlayerID);
								}
								MobaGameState->SettleDeath(InDamageCauser->GetPlayerID(), PlayerID);
		
								//复活
								//GThread::GetCoroutines().BindUObject(RebornTime, this, &AMobaCharacter::Multicast_Reborn);
								//死亡2s后销毁
								GThread::GetCoroutines().BindLambda(2.0f, [&]()
								{
									Destroy();
								});
							} 
							else //受伤
							{
								//添加助攻
								//注意这里不能直接用GetPlayerState获取MobaPlayState，因为MobaPlayState绑定的是MobaPawn，而不是MobaCharacter
								if(AMobaPlayerState* MobaPlayState = MethodUnit::GetMobaPlayerStateFromPlayerID(GetWorld(), PlayerID))
								{
									MobaPlayState->AddAssistPlayer(InDamageCauser->GetPlayerID());
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

void AMobaCharacter::Multicast_StatusBar_Implementation(float HealthPercent, float ManaPercent)
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

void AMobaCharacter::Multicast_StatusBar_Health_Implementation(float HealthPercent)
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

		//死亡后隐藏状态栏
		if(HealthPercent <= 0.0f)
		{
			StatusBarComponent->SetVisibility(false);
		}
	}
}

void AMobaCharacter::Multicast_StatusBar_Mana_Implementation(float ManaPercent)
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

void AMobaCharacter::InitHealthBarColorfromTeamType()
{
	if(const AMobaPlayerState* MobaPlayerState = MethodUnit::GetMobaPlayerState(GetWorld()))
	{
		//队伍血条颜色区分
		if(const UUI_StatusBar_Health* StatusBarUI_Health = Cast<UUI_StatusBar_Health>(StatusBarComponent->GetUserWidgetObject()))
		{
			//PlayerDataComponet上的TeamType是本地玩家的队伍
			if(MobaPlayerState->GetPlayerDataComponent()->TeamType == this->TeamType) //友军绿色
			{
				StatusBarUI_Health->SetColor(FLinearColor::Green);
			}
			else //敌方队伍 + 中立 红色
			{
				StatusBarUI_Health->SetColor(FLinearColor::Red);
			}
		}
	}
	else
	{
		GThread::GetCoroutines().BindUObject(0.3f, this, &AMobaCharacter::InitHealthBarColorfromTeamType);
	}
}

void AMobaCharacter::Multicast_Reborn_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		if(FCharacterAttribute* CharacterAttribute = GetCharacterAttribute())
		{
			//重置属性
			CharacterAttribute->ResetAttribute();
			
			//更新血条蓝条
			Multicast_StatusBar(CharacterAttribute->GetHealthPercent(), CharacterAttribute->GetManaPercent());
			
			//更新属性面板
			if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
			{
				MobaGameState->RequestUpdateCharacterAttribute(PlayerID, PlayerID,ECharacterAttributeType::ECAT_CurrentHealth);
				MobaGameState->RequestUpdateCharacterAttribute(PlayerID, PlayerID,ECharacterAttributeType::ECAT_CurrentMana);
			}
		}
	}
	else
	{
		StatusBarComponent->SetVisibility(true); //出生后显示状态栏
	}
	
	StopAnimMontage(); //停止死亡动画
}

void AMobaCharacter::Multicast_SpwanDrawText_Implementation(float Value, float Percent, const FLinearColor& Color,
	const FVector& Location)
{
	if(ADrawText* DrawText = GetWorld()->SpawnActor<ADrawText>(DrawTextClass, Location, FRotator::ZeroRotator))
	{
		DrawText->SetTextBlock(FString::Printf(TEXT("%d"), static_cast<int>(Value)), Color, Percent);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("error"));
	}
}

void AMobaCharacter::Multicast_SpawnHitVFX_Implementation(const FVector& HitLocation, UParticleSystem* HitVFX)
{
	//只在客户端生成特效
	if(GetLocalRole() != ROLE_Authority)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitVFX, HitLocation);
	}
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
	if(const FCharacterAttribute* CharacterAttribute = GetCharacterAttribute())
	{
		GetCharacterMovement()->MaxWalkSpeed = CharacterAttribute->WalkSpeed;  //设置移动速度
		Multicast_StatusBar(CharacterAttribute->GetHealthPercent(), CharacterAttribute->GetManaPercent()); // 广播状态栏
	}
}

void AMobaCharacter::InitWidgetInfo()
{
	Multicast_StatusBar(GetCharacterAttribute()->GetHealthPercent(), GetCharacterAttribute()->GetManaPercent());
}

FCharacterAttribute* AMobaCharacter::GetCharacterAttribute() const
{
	return MethodUnit::GetCharacterAttributeFromPlayerID(GetWorld(), PlayerID);
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

void AMobaCharacter::AddBulletPtr(ABullet* InBullet)
{
	if(BulletPtrs.Contains(InBullet))
	{
		BulletPtrs.Add(InBullet);
	}
}

void AMobaCharacter::CallBulletEndFire()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		if(!BulletPtrs.IsEmpty())
		{
			TArray<TWeakObjectPtr<ABullet>> RemovedPtrs;
		
			for(auto& BulletPtr : BulletPtrs)
			{
				if(BulletPtr.IsValid())
				{
					BulletPtr->Multicast_EndOpenFireVFX();
				}
				else
				{
					//收集无效指针
					RemovedPtrs.Add(BulletPtr);
				}
			}

			for(auto& RemovedPtr : RemovedPtrs)
			{
				BulletPtrs.Remove(RemovedPtr);
			}
		}
	}
}

void AMobaCharacter::LockRotate(bool InbLockRotate)
{
	bLockRotate = InbLockRotate;
}

void AMobaCharacter::ResetRotation()
{
	CurrentRotation = GetActorRotation();
}


void AMobaCharacter::Multicast_PlayerAnimMontage_Implementation(UAnimMontage* InAnimMontage, float InPlayRate,
                                                                FName StartSectionName)
{
	if(InAnimMontage)
	{
		PlayAnimMontage(InAnimMontage, InPlayRate, StartSectionName);
		//是否完成播放
	}
}

