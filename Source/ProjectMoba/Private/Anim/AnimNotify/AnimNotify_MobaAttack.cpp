// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/AnimNotify_MobaAttack.h"

#include "Common/MethodUnit.h"
#include "Item/Bullet.h"

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
	Super::Notify(MeshComp, Animation, EventReference);
	
	if(BulletClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BulletClass为空, 请在AnimNotify_Attack中设置BulletClass"));
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
		/** 在服务器上生成子弹 */
		/** 在编辑器上用前一句判断，Runtime用后一句判断 */
		if(Character->GetWorld()->IsNetMode(NM_DedicatedServer) || Character->GetLocalRole() == ROLE_Authority)
		{
			// 子弹来源
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Character;  // 子弹的拥有者
			SpawnParams.Instigator = Cast<APawn>(Character); // 子弹的发起者

			// 生成子弹
			if(ABullet* Bullet = Character->GetWorld()->SpawnActor<ABullet>(BulletClass, ComponentLocation, ComponentRotation, SpawnParams))
			{
				Bullet->SetRangeCheck(false);
				Bullet->SetLifeSpan(LifeSpan);
			}
		}
	}

	
}
