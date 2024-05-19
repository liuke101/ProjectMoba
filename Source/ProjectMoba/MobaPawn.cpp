#include "ProjectMoba/MobaPawn.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectMoba/ProjectMobaCharacter.h"

AMobaPawn::AMobaPawn()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBox"));
	RootComponent = RootBox;
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMobaPawn::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority) // 相当于 if (HasAuthority())
	{
		if(DefaultPawnClass)
		{
			MobaCharacter = GetWorld()->SpawnActor<AProjectMobaCharacter>(DefaultPawnClass, GetActorLocation(), GetActorRotation());
		}
	}
	
}

void AMobaPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(GetLocalRole() == ROLE_Authority)
	{
		if(DefaultPawnClass)
		{
			if(MobaCharacter)
			{
				MobaCharacter->Destroy();
				MobaCharacter = nullptr;
			}
			
		}
	}
}

void AMobaPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMobaPawn::CharacterMoveToOnServer_Implementation(const FVector& Destination)
{
	if(MobaCharacter)
	{
		const float Distance = FVector::Dist(MobaCharacter->GetActorLocation(), Destination);
		if(Distance>120.0f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(MobaCharacter->GetController(), Destination);
		}
	}
}

