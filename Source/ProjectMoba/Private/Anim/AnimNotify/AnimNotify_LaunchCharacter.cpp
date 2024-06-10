// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotify_LaunchCharacter.h"

#include "AI/MobaAIController.h"
#include "Character/MobaCharacter.h"

UAnimNotify_LaunchCharacter::UAnimNotify_LaunchCharacter()
{
}

void UAnimNotify_LaunchCharacter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(MeshComp->GetOuter()))
	{
		if(OwnerCharacter->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			if(AMobaAIController* AIController = OwnerCharacter->GetController<AMobaAIController>())
			{
				FVector NewPos = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * LaunchDistance;
				OwnerCharacter->SetActorLocation(NewPos);
				AIController->MoveToLocation(NewPos);
			}
		}
	}
}
