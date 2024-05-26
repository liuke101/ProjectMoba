// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotify_MobaAttack.h"

#include "Common/MethodUnit.h"
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
	
	FVector ComponentLocation = FVector::ZeroVector; 
    FRotator ComponentRotation = FRotator::ZeroRotator;
	
#if WITH_EDITOR // 编辑器通过Socket获取组件位置
	ComponentLocation = MeshComp->GetSocketLocation(SocketName);
	ComponentRotation = MeshComp->GetSocketRotation(SocketName);
#else // Runtime直接通过组件获取组件位置
	if(AMobaCharacter* Character = Cast<AMobaCharacter>(MeshComp->GetOwner()))
	{
		ComponentLocation = Character->GetFirePointLocation();
		ComponentRotation = Character->GetFirePointRotation();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("未能获取到 FirePointLocation"));
		return;
	}
#endif

	if(AActor* Character = Cast<AActor>(MeshComp->GetOuter()))
	{
		/** 在服务器上生成damagebox */
		/** 在编辑器上用前一句判断，Runtime用后一句判断 */
		if(Character->GetWorld()->IsNetMode(NM_DedicatedServer) || Character->GetLocalRole() == ROLE_Authority)
		{
			// 来源
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Character;  // damagebox的拥有者
			SpawnParams.Instigator = Cast<APawn>(Character); // damagebox的发起者

			
			if(ADamageBox* DamageBoxInstance = Character->GetWorld()->SpawnActor<ADamageBox>(DamageBoxClass, ComponentLocation, ComponentRotation, SpawnParams))
			{
				// 如果生成的是子弹
				if(ABullet* Bullet = Cast<ABullet>(DamageBoxInstance))
				{
					Bullet->SetRangeCheck(false);
					Bullet->SetLifeSpan(LifeSpan);
					//Bullet其他设置
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

	
}
