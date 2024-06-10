// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotifyState_LockCharacterRotate.h"

#include "Character/MobaCharacter.h"

void UAnimNotifyState_LockCharacterRotate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(MeshComp->GetOwner()))
	{
		if(OwnerCharacter->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			OwnerCharacter->ResetRotation();
			OwnerCharacter->LockRotate(true);
		}
	}
}

void UAnimNotifyState_LockCharacterRotate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(MeshComp->GetOwner()))
	{
		if(OwnerCharacter->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			OwnerCharacter->LockRotate(false);
		}
	}
}
