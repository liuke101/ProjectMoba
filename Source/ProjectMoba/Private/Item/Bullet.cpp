// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Bullet.h"

#include "AI/MobaAIController.h"
#include "Character/MobaCharacter.h"
#include "Common/CalculationUnit.h"
#include "Components/BoxComponent.h"
#include "Game/MobaGameMode.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; //开启复制

	RootBullet = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = RootBullet;
	
	BoxDamage = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxDamage->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 1600.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; //无重力
	ProjectileMovementComponent->UpdatedComponent = BoxDamage;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	// 只在服务器上注册重叠事件
	if(GetLocalRole() == ROLE_Authority)
	{
		BoxDamage->OnComponentBeginOverlap.AddDynamic(this, &ABullet::BeginOverlap);
	}
	
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AMobaCharacter* InstigatorCharacter = Cast<AMobaCharacter>(GetInstigator()))
	{
		if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(OtherActor))
		{
			float DamgeValue = CalculationUnit::GetTotalDamage(TargetCharacter, InstigatorCharacter);
			//排除自己和友军
			if(InstigatorCharacter != TargetCharacter && InstigatorCharacter->GetTeamType() != TargetCharacter->GetTeamType()) 
			{
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
				
				//造成伤害
				UGameplayStatics::ApplyDamage(
					TargetCharacter,
					DamgeValue,
					InstigatorCharacter->GetController(),
					InstigatorCharacter,
					UDamageType::StaticClass());

				//销毁子弹
				Destroy();
			}
		}
	}
}

