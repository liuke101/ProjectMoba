// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaPlayerController.h"

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/MobaPawn.h"

#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectMoba/GlobalVariable.h"
#include "Tool/ScreenMoveUnits.h"

AMobaPlayerController::AMobaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AMobaPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if(GetLocalRole() == ROLE_AutonomousProxy)
	{
		//屏幕移动插件
		FScreenMoveUnits().ListenScreenMove(this, 10.0f);
	}
}

void AMobaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMobaPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AMobaPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AMobaPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AMobaPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AMobaPlayerController::OnSetDestinationReleased);
	}
}

void AMobaPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AMobaPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	//if(GetLocalRole()==ROLE_AutonomousProxy)
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			FVector2D MousePosition;
			if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
			{
				// MousePosition为客户端鼠标在屏幕上的位置，需要转换成世界坐标传给服务端(参考GetHitResultAtScreenPosition函数的实现)
				if (GetHUD() != NULL && GetHUD()->GetHitBoxAtCoordinates(MousePosition, true))
				{
					return ;
				}

				FVector WorldOrigin;
				FVector WorldDirection;
				if (UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection) == true)
				{
					VerifyMouseWorldPositionClickOnServer(WorldOrigin, WorldDirection);
					//MobaPawn->CharacterMoveToOnServer(Hit.Location);
					//CachedDestination = Hit.Location;
				}
			}
		}
	}
}

void AMobaPlayerController::OnSetDestinationReleased()
{
	if(GetLocalRole()==ROLE_AutonomousProxy)
	{
		// If it was a short press
		if (FollowTime <= ShortPressThreshold)
		{
			// We move there and spawn some particles
			Cast<AMobaPawn>(GetPawn())->CharacterMoveToOnServer(CachedDestination); //Server RPC
			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		
		}
	}

	FollowTime = 0.f;
}

bool AMobaPlayerController::VerifyMouseWorldPositionClickOnServer_Validate(const FVector& WorldOrigin,
	const FVector& WorldDirection)
{
	return true;
}

void AMobaPlayerController::VerifyMouseWorldPositionClickOnServer_Implementation(const FVector& WorldOrigin,
                                                                                        const FVector& WorldDirection)
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), false);
		auto TracePos = [&](ECollisionChannel EccChannel)->bool
		{
			return GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, EccChannel, CollisionQueryParams);
		};

		if(TracePos(ECC_Character)) 
		{
			if(HitResult.bBlockingHit) // 检测到就移动攻击
			{
				MobaPawn->CharacterMoveToTargetWithAttackOnServer(HitResult.ImpactPoint, Cast<AMobaPawn>(HitResult.GetActor()));
			}
		}

		if(TracePos(ECC_Visibility))
		{
			MobaPawn->CharacterMoveToOnServer(HitResult.ImpactPoint);
		}
	}

}
