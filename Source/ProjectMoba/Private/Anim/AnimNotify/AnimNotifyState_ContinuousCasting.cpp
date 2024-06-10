// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotifyState_ContinuousCasting.h"

#include "Character/MobaCharacter.h"
#include "Character/Hero/MobaHeroCharacter.h"

void UAnimNotifyState_ContinuousCasting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	bEndContinuousAnim = false;
	MobaHero = Cast<AMobaHeroCharacter>(MeshComp->GetOuter());
}

void UAnimNotifyState_ContinuousCasting::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(!bEndContinuousAnim)
	{
		if(MobaHero && MobaHero->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			//如果结束时技能键还被按下，则继续播放Loop
			if(MobaHero->PressSkillKey)
			{
				MobaHero->Multicast_PlayerAnimMontage(LoopAnim);
			}
			else //如果松开，则结束
			{
				bEndContinuousAnim = true; //结束tick
				MobaHero->Multicast_PlayerAnimMontage(EndAnim);
				MobaHero->CallBulletEndFire(); 
			}
		}
	}
}

void UAnimNotifyState_ContinuousCasting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if(!bEndContinuousAnim)
	{
		if(MobaHero && MobaHero->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			//如果技能键被松开，则结束施法
			if(!MobaHero->PressSkillKey)
			{
				bEndContinuousAnim = true; //结束tick
				MobaHero->Multicast_PlayerAnimMontage(EndAnim);
				MobaHero->CallBulletEndFire(); 
			}
		}
	}
}

FString UAnimNotifyState_ContinuousCasting::GetNotifyName_Implementation() const
{
	return "ContinuousCasting";
}
