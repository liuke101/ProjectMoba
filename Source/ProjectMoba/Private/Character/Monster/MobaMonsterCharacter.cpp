// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/MobaMonsterCharacter.h"

#include "AI/MobaMonsterAIController.h"


AMobaMonsterCharacter::AMobaMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMobaMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMobaMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobaMonsterCharacter::InitCharacter()
{
	Super::InitCharacter();

	// 设置怪物的出生点
	if(AMobaMonsterAIController* MobaMonsterAIController = GetController<AMobaMonsterAIController>())
	{
		MobaMonsterAIController->HomePoint = GetActorLocation();
	}
	
}

float AMobaMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                        AController* EventInstigator, AActor* DamageCauser)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		// 将攻击目标设置为伤害来源
		if(AMobaCharacter* MobaDamgeCauser = Cast<AMobaCharacter>(DamageCauser))
		{
			if(AMobaMonsterAIController* MobaMonsterAIController = GetController<AMobaMonsterAIController>())
			{
				//当没有目标且不在回家过程中才会选择新目标
				if(!MobaMonsterAIController->GetTarget() && !MobaMonsterAIController->bHoming)
				{
					MobaMonsterAIController->SetTarget(MobaDamgeCauser);
				}
			}
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

