// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotify_MobaAttack.h"

#include "AI/MobaAIController.h"
#include "Character/MobaCharacter.h"
#include "Item/Bullet.h"
#include "Item/DamageBox.h"

UAnimNotify_MobaAttack::UAnimNotify_MobaAttack()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Red;
#endif
	
}

FString UAnimNotify_MobaAttack::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_MobaAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if(DamageBoxClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("DamageBoxClass为空, 请在AnimNotify_Attack中设置DamageBoxClass"));
		return;
	}
	
	if(AMobaCharacter* OwnerCharacter = Cast<AMobaCharacter>(MeshComp->GetOuter()))
	{
		FVector ComponentLocation = MeshComp->GetSocketLocation(SocketName);
		FRotator ComponentRotation =MeshComp->GetSocketRotation(SocketName);

		if(OwnerCharacter->GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			if(AMobaAIController* AIController = Cast<AMobaAIController>(OwnerCharacter->GetController()))
			{
				if(AMobaCharacter* Target = AIController->GetTarget())
				{
					// 开火点朝向目标
					ComponentRotation = (Target->GetActorLocation() - ComponentLocation).ToOrientationRotator();
					if(OwnerCharacter->GetActorRotation() != FRotator::ZeroRotator)
					{
						ComponentRotation -= OwnerCharacter->GetActorRotation();
					}
				}
			}
		}

		// 来源
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;  // damagebox的拥有者
		SpawnParams.Instigator = Cast<APawn>(OwnerCharacter); // damagebox的发起者
		
		if(ADamageBox* DamageBoxInstance = OwnerCharacter->GetWorld()->SpawnActor<ADamageBox>(DamageBoxClass, ComponentLocation, ComponentRotation, SpawnParams))
		{
			// 如果生成的是子弹
			if(ABullet* Bullet = Cast<ABullet>(DamageBoxInstance))
			{
				Bullet->SetRangeCheck(false);
				Bullet->SetLifeSpan(LifeSpan);
				//Bullet其他设置
				// 
				// 	Bullet->SetHomingTarget(AIController->GetTarget());
			}
			else // 如果生成的是DamageBox
			{
				DamageBoxInstance->SetRangeCheck(false);
				DamageBoxInstance->SetLifeSpan(LifeSpan);
				//DamageBox其他设置
			}
		}
	}
}
