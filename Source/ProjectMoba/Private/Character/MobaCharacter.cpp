// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaCharacter.h"

#include "Common/MethodUnit.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Table/CharacterAsset.h"

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
		SpawnDefaultController(); //生成AIController
	}
}

void AMobaCharacter::NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget)
{
	if(InTarget.IsValid())
	{
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

void AMobaCharacter::RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID)
{
	SetPlayerID(InPlayerID);

	
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		MobaGameState->Add_PlayerID_To_CharacterAttribute(InPlayerID, InCharacterID);
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

