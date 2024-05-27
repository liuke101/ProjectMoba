// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotifyState_StopAttackingRotate.h"

#include "AI/MobaAIController.h"
#include "GameFramework/Character.h"

void UAnimNotifyState_StopAttackingRotate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if(Character->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			if(AMobaAIController* AIController = Character->GetController<AMobaAIController>())
			{
				AIController->StopAttackingRotate(true);
			}
		}
	}
}

void UAnimNotifyState_StopAttackingRotate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if(ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if(Character->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			if(AMobaAIController* AIController = Character->GetController<AMobaAIController>())
			{
				AIController->StopAttackingRotate(false);
			}
		}
	}
}
