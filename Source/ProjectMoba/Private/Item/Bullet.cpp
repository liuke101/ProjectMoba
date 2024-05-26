// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Bullet.h"

#include "GameFramework/ProjectileMovementComponent.h"


ABullet::ABullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 1600.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; //无重力
	ProjectileMovementComponent->UpdatedComponent = RootComponent;

	// // 开启追踪
	// ProjectileMovementComponent->bIsHomingProjectile = true; 
	// // 设置加速度
	// ProjectileMovementComponent->HomingAccelerationMagnitude = 100.0f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// void ABullet::SetHomingTarget(const AMobaCharacter* Target) const
// {
// 	if(Target)
// 	{
// 		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
// 	}
// }


