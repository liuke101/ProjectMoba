

#include "Character/MobaPawn.h"

#include "AI/MobaAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Character/MobaCharacter.h"
#include "Common/MethodUnit.h"
#include "Components/BoxComponent.h"
#include "Game/MobaGameState.h"
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

	if(GetLocalRole() == ROLE_Authority) 
	{
		// 从txt读取角色ID，然后根据角色ID生成角色
		if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
		{
			FString NumberString;
			FFileHelper::LoadFileToString(NumberString, *(FPaths::ProjectDir() / TEXT("CharacterID.txt"))); //从txt读取角色ID
			const int32 CharacterID = FCString::Atoi64(*NumberString);
			
			if(const FCharacterTable* CharacterTable = MobaGameState->GetCharacterTable(CharacterID))
			{
				DefaultCharacterClass = CharacterTable->CharacterClass;
			}

			if(DefaultCharacterClass)
			{
				MobaCharacter = GetWorld()->SpawnActor<AMobaCharacter>(DefaultCharacterClass, GetActorLocation(), GetActorRotation());
				if(MobaCharacter)
				{
					MobaCharacter->InitCharacterID(CharacterID);
				}
			}
		}
		
		
	}
	
}

void AMobaPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(GetLocalRole() == ROLE_Authority)
	{
		if(DefaultCharacterClass)
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

void AMobaPawn::SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaCharacter> InTarget) const
{
	if(MobaCharacter)
	{
		MobaCharacter->SkillAttack(SkillKey, InTarget);
	}
}

bool AMobaPawn::CharacterMoveToTargetWithAttackOnServer_Validate(const FVector& Destination, const APawn* TargetPawn)
{
	// 如果目标角色不为空，且目标角色不是自己则通过验证
	//return TargetPawn != nullptr && TargetPawn != MobaCharacter;
	return true;
}

void AMobaPawn::CharacterMoveToTargetWithAttackOnServer_Implementation(const FVector& Destination,
                                                                       const APawn* TargetPawn)
{
	if(MobaCharacter)
	{
		if(AMobaAIController* MobaAIController = Cast<AMobaAIController>(MobaCharacter->GetController()))
		{
			MobaAIController->SetTarget(Cast<AMobaCharacter>(const_cast<APawn*>(TargetPawn)));
		}
	}
}

void AMobaPawn::CharacterMoveToOnServer_Implementation(const FVector& Destination)
{
	if(MobaCharacter)
	{
		const float Distance = FVector::Dist(MobaCharacter->GetActorLocation(), Destination);
		//打印Destination
		if(Distance > 120.0f)
		{
			if(AMobaAIController* MobaAIController = Cast<AMobaAIController>(MobaCharacter->GetController()))
			{
				MobaAIController->MoveToLocation(Destination);
				///UAIBlueprintHelperLibrary::SimpleMoveToLocation(MobaAIController,Destination);
			}
		}
	}
}

