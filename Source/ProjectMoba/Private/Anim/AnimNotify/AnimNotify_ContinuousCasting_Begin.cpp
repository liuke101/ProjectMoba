// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotify_ContinuousCasting_Begin.h"

#include "Character/MobaCharacter.h"

FString UAnimNotify_ContinuousCasting_Begin::GetNotifyName_Implementation() const
{
	return FString("ContinuousCasting_Begin");
}

void UAnimNotify_ContinuousCasting_Begin::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!LoopAnim)
	{
		return;
	}

	if (AMobaCharacter* Character = Cast<AMobaCharacter>(MeshComp->GetOuter()))
	{
		if (Character->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			Character->Multicast_PlayerAnimMontage(LoopAnim);
		}
	}
}
