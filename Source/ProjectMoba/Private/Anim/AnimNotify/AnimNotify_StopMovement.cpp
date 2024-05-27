﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotify_StopMovement.h"

#include "Character/MobaCharacter.h"

void UAnimNotify_StopMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(AMobaCharacter* MobaCharacter = Cast<AMobaCharacter>(MeshComp->GetOuter()))
	{
		if(MobaCharacter->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			MobaCharacter->ResetSpeed(0.0f); //停止移动
		}
	}

	
}

void UAnimNotify_StopMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(AMobaCharacter* MobaCharacter = Cast<AMobaCharacter>(MeshComp->GetOuter()))
	{
		if(MobaCharacter->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			MobaCharacter->ResetSpeed(-1);  //恢复原速度
		}
	}
}