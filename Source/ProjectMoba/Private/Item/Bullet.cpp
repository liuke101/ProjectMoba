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
	bReplicates = true; //开启复制

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 1600.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; //无重力
	ProjectileMovementComponent->UpdatedComponent = DamageBox;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

