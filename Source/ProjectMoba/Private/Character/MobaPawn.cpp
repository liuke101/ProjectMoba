

#include "Character/MobaPawn.h"
#include "AI/MobaAIController.h"
#include "Camera/CameraComponent.h"
#include "Character/MobaCharacter.h"
#include "Character/Hero/MobaHeroCharacter.h"
#include "Common/MethodUnit.h"
#include "Component/PlayerDataComponent.h"
#include "Components/BoxComponent.h"
#include "Game/MobaPlayerState.h"
#include "GameFramework/SpringArmComponent.h"


AMobaPawn::AMobaPawn()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBox"));
	RootComponent = RootBox;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); 
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; 

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; 
	
	// Tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMobaPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMobaPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(GetLocalRole() == ROLE_Authority)
	{
		if(ControlledMobaHero)
		{
			ControlledMobaHero->Destroy();
			ControlledMobaHero = nullptr;
		}
	}
}

void AMobaPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMobaPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMobaPawn::SkillAttack(int32 SkillDataID) const
{
	if(ControlledMobaHero)
	{
		ControlledMobaHero->SkillAttack(SkillDataID);
	}
}

int64 AMobaPawn::GetPlayerID() 
{
	if(const UPlayerDataComponent* PlayerDataComponent = GetPlayerDataComponent())
	{
		return PlayerDataComponent->PlayerID;
	}
	
	return INDEX_NONE;
}

UPlayerDataComponent* AMobaPawn::GetPlayerDataComponent() const
{
	if(AMobaPlayerState* MobaPlayerState = GetPlayerState<AMobaPlayerState>())
	{
		return MobaPlayerState->GetPlayerDataComponent();
	}
	return nullptr;
}

void AMobaPawn::Server_CharacterMoveToTargetWithAttack_Implementation(const FVector& Destination,
                                                                       const APawn* TargetPawn)
{
	if(ControlledMobaHero)
	{
		if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(const_cast<APawn*>(TargetPawn)))
		{
			//友军检测
			if(MethodUnit::IsFriendly(ControlledMobaHero, TargetCharacter)) return;
			
			if(AMobaAIController* MobaAIController = Cast<AMobaAIController>(ControlledMobaHero->GetController()))
			{
				MobaAIController->SetTarget(TargetCharacter);
			}
		}
	}
}

void AMobaPawn::Server_CharacterMoveTo_Implementation(const FVector& Destination)
{
	if(ControlledMobaHero)
	{
		const float Distance = FVector::Dist(ControlledMobaHero->GetActorLocation(), Destination);
		
		if(Distance > 120.0f)
		{
			if(AMobaAIController* MobaAIController = Cast<AMobaAIController>(ControlledMobaHero->GetController()))
			{
				MobaAIController->MoveToLocation(Destination);
			}
		}
	}
}

