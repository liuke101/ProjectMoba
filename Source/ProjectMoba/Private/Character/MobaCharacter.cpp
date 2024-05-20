// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaCharacter.h"

#include "Common/MethodUnit.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"

AMobaCharacter::AMobaCharacter()
	: bAttacking(false),
	  AttackCount(0),
	  CharacterID(INDEX_NONE)
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
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMobaCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMobaCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(GetLocalRole()==ROLE_Authority)
	{
		SpawnDefaultController();
	}
}

void AMobaCharacter::NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget)
{
	if(InTarget.IsValid())
	{
		if(const FCharacterTable* CharacterTable = MethodUnit::GetMobaGameState(GetWorld())->GetCharacterTable(CharacterID))
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
	if(const FCharacterTable* CharacterTable = MethodUnit::GetMobaGameState(GetWorld())->GetCharacterTable(CharacterID))
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
	if(const FCharacterTable* CharacterTable = MethodUnit::GetMobaGameState(GetWorld())->GetCharacterTable(CharacterID))
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

void AMobaCharacter::InitCharacterID(const int64& InCharacterID)
{
	CharacterID = InCharacterID;
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

