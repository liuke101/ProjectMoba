// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotifyWindow_ContinuousCasting.h"

#include "Character/MobaCharacter.h"
#include "Character/Hero/MobaHeroCharacter.h"

void UAnimNotifyWindow_ContinuousCasting::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
	
	bEndContinuousAnim = false;
	MobaHero = Cast<AMobaHeroCharacter>(BranchingPointPayload.SkelMeshComponent->GetOuter());
}

void UAnimNotifyWindow_ContinuousCasting::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

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
			}
		}
	}
}

void UAnimNotifyWindow_ContinuousCasting::BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload,
	float FrameDeltaTime)
{
	Super::BranchingPointNotifyTick(BranchingPointPayload, FrameDeltaTime);

	if(!bEndContinuousAnim)
	{
		if(MobaHero && MobaHero->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			//如果技能键被松开，则结束施法
			if(!MobaHero->PressSkillKey)
			{
				bEndContinuousAnim = true; //结束tick
				MobaHero->Multicast_PlayerAnimMontage(EndAnim);
			}
		}
	}
}

FString UAnimNotifyWindow_ContinuousCasting::GetNotifyName_Implementation() const
{
	return "ContinuousCasting";
}
