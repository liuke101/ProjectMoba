// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"

AMobaCharacter::AMobaCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMobaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole()==ROLE_AutonomousProxy)
	{
		SpawnDefaultController();
	}
}

void AMobaCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMobaCharacter::NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget)
{
	if(InTarget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("NormalAttack"));
	}
}

