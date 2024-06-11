#include "Item/DamageBox.h"

#include "AI/MobaAIController.h"
#include "Character/MobaCharacter.h"
#include "Common/CalculationUnit.h"
#include "Common/MethodUnit.h"
#include "Common/MobaDamageType.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


ADamageBox::ADamageBox()
{
	PrimaryActorTick.bCanEverTick = true;

	bNetLoadOnClient = true; //不在客户端加载
	bReplicates = true; //关闭复制
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = Root;
	
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	DamageBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//设置碰撞属性
	DamageBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	OpenFirePointVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OpenFirePointVFX"));
	OpenFirePointVFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	OpenFireBullet = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OpenFireBullet"));
	OpenFireBullet->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ADamageBox::BeginPlay()
{
	Super::BeginPlay();
	
	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &ADamageBox::BeginOverlap);
}

void ADamageBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADamageBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AMobaCharacter* InstigatorCharacter = Cast<AMobaCharacter>(GetInstigator()))
	{
		if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(OtherActor))
		{
			//服务器端
			if(GetWorld()->IsNetMode(NM_DedicatedServer))
			{
				//排除死亡角色
				if(TargetCharacter->IsDead()) return;
				//排除自己和友军
				if(InstigatorCharacter == TargetCharacter && MethodUnit::IsFriendly(InstigatorCharacter, TargetCharacter)) return;
				
				//单体目标检测
				if(bSingleCheck)
				{
					if(AMobaAIController* AIController = Cast<AMobaAIController>(InstigatorCharacter->GetController()))
					{
						if(AIController->GetTarget() != TargetCharacter) 
						{
							return;
						}
					}
				}

				// 生成命中特效
				if(UParticleSystem* HitVFX = GetHitVFX())
				{
					TargetCharacter->Multicast_SpawnHitVFX(TargetCharacter->GetActorLocation(), HitVFX);
				}


				if(UClass* MobaDmageTypeClass = UMobaDamageType::StaticClass())
				{
					if(UMobaDamageType* MobaDmageType = MobaDmageTypeClass->GetDefaultObject<UMobaDamageType>())
					{
						MobaDmageType->SlotAttribute = SlotAttribute;
				
						//造成伤害
						UGameplayStatics::ApplyDamage(
							TargetCharacter,
							CalculationUnit::GetTotalDamage(TargetCharacter, InstigatorCharacter),
							InstigatorCharacter->GetController(),
							InstigatorCharacter,
							MobaDmageTypeClass);
					}
				}

				//销毁
				if(bSingleTarget)
				{
					Destroy();
				}
			}
		}
	}
}

UParticleSystem* ADamageBox::GetHitVFX() const
{
	if(HitVFXs.Num() > 0)
	{
		return HitVFXs[FMath::RandRange(0, HitVFXs.Num() - 1)];
	}
	return nullptr;

}

void ADamageBox::SetBoxSize(const FVector& Size)
{
	DamageBox->SetBoxExtent(Size);
}

void ADamageBox::SetRelativePositon(const FVector& Position)
{
	DamageBox->SetRelativeLocation(Position);
}

void ADamageBox::SetOpenFireActive(bool bActive) const
{
	OpenFirePointVFX->SetVisibility(bActive);
	OpenFireBullet->SetVisibility(bActive);
}

void ADamageBox::Multicast_EndOpenFireVFX_Implementation()
{
	if(GetLocalRole() != ROLE_Authority)
	{
		SetOpenFireActive(false);
	}
}


