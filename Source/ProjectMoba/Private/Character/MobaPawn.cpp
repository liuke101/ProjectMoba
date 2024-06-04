

#include "Character/MobaPawn.h"

#include "AI/MobaAIController.h"
#include "Camera/CameraComponent.h"
#include "Character/MobaCharacter.h"
#include "Character/Hero/MobaHeroCharacter.h"
#include "Common/MethodUnit.h"
#include "Component/PlayerDataComponent.h"
#include "Components/BoxComponent.h"
#include "Game/MobaGameState.h"
#include "Game/MobaPlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Table/CharacterAsset.h"


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
	 	
	 }
}

void AMobaPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(GetLocalRole() == ROLE_Authority)
	{
		if(DefaultCharacterClass)
		{
			if(ControlledMobaHero)
			{
				ControlledMobaHero->Destroy();
				ControlledMobaHero = nullptr;
			}
		}
	}
}

void AMobaPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 从txt读取角色ID，然后根据角色ID生成角色
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		FString NumberString;
		FFileHelper::LoadFileToString(NumberString, *(FPaths::ProjectDir() / TEXT("CharacterID.txt"))); 
		int32 CharacterID = FCString::Atoi(*NumberString);  
	 		
		if(const FCharacterAsset* CharacterAsset = MobaGameState->GetCharacterAssetFromCharacterID(CharacterID))
		{
			DefaultCharacterClass = CharacterAsset->CharacterClass;
			if(DefaultCharacterClass)
			{
				// 服务器上生成的Character可能还未在客户端生成（同步），就调用了广播MulticastStatusBar，导致客户端无法同步状态栏信息。解决方法，在注册时用计时器短暂延迟。(或者在AIController的OnRep_Pawn中执行？）
				// 此外，刚生成Character时，Pawn可能还未被PlayerController持有，导致无法在 GetPlayerID 函数中获取 PlayerState。所以不能再BeginPlay中执行，在PossessBy中执行可以解决该问题。
				ControlledMobaHero = GetWorld()->SpawnActor<AMobaHeroCharacter>(DefaultCharacterClass, GetActorLocation(), GetActorRotation());

				/** 在服务器上注册角色信息 */
				if(ControlledMobaHero)
				{
					int64 PlayerID = GetPlayerID();
					if(PlayerID != INDEX_NONE)
					{
						ControlledMobaHero->RegisterCharacterOnServer(PlayerID , CharacterID, GetPlayerDataComponent()->TeamType, CharacterAsset->CharacterType);
					}
				}
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
	//if(AMobaPlayerState* MobaPlayerState = GetController()->GetPlayerState<AMobaPlayerState>())
	if(AMobaPlayerState* MobaPlayerState = GetPlayerState<AMobaPlayerState>())
	{
		return MobaPlayerState->GetPlayerDataComponent();
	}
	return nullptr;
}

bool AMobaPawn::Server_CharacterMoveToTargetWithAttack_Validate(const FVector& Destination, const APawn* TargetPawn)
{
	// 如果目标角色不为空，且目标角色不是自己则通过验证
	//return TargetPawn != nullptr && TargetPawn != MobaCharacter;
	return true;
}

void AMobaPawn::Server_CharacterMoveToTargetWithAttack_Implementation(const FVector& Destination,
                                                                       const APawn* TargetPawn)
{
	if(ControlledMobaHero)
	{
		if(AMobaAIController* MobaAIController = Cast<AMobaAIController>(ControlledMobaHero->GetController()))
		{
			MobaAIController->SetTarget(Cast<AMobaCharacter>(const_cast<APawn*>(TargetPawn)));
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

