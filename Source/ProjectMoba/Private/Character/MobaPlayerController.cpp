// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaPlayerController.h"

#include "GameFramework/Pawn.h"
#include "NiagaraFunctionLibrary.h"

#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/MobaCharacter.h"
#include "Character/MobaPawn.h"
#include "Character/Hero/MobaHero.h"

#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectMoba/GlobalVariable.h"
#include "Tool/ScreenMoveUnits.h"

AMobaPlayerController::AMobaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMobaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if(GetLocalRole() == ROLE_AutonomousProxy)
	{
		if(bMoveToMouseCursor)
		{
			MoveToMouseCursor();
		}
		
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
		EnhancedInputComponent->BindAction(RightClick_Action, ETriggerEvent::Started, this, &AMobaPlayerController::OnRightClickPressed);
		EnhancedInputComponent->BindAction(RightClick_Action, ETriggerEvent::Completed, this, &AMobaPlayerController::OnRightClickReleased);
		EnhancedInputComponent->BindAction(W_Action, ETriggerEvent::Started, this, &AMobaPlayerController::OnWPressed);
		EnhancedInputComponent->BindAction(W_Action, ETriggerEvent::Completed, this, &AMobaPlayerController::OnWReleased);
		EnhancedInputComponent->BindAction(E_Action, ETriggerEvent::Started, this, &AMobaPlayerController::OnEPressed);
		EnhancedInputComponent->BindAction(E_Action, ETriggerEvent::Completed, this, &AMobaPlayerController::OnEReleased);
		EnhancedInputComponent->BindAction(R_Action, ETriggerEvent::Started, this, &AMobaPlayerController::OnRPressed);
		EnhancedInputComponent->BindAction(R_Action, ETriggerEvent::Completed, this, &AMobaPlayerController::OnRReleased);
		EnhancedInputComponent->BindAction(F_Action, ETriggerEvent::Started, this, &AMobaPlayerController::OnFPressed);
		EnhancedInputComponent->BindAction(F_Action, ETriggerEvent::Completed, this, &AMobaPlayerController::OnFReleased);
		EnhancedInputComponent->BindAction(Space_Action, ETriggerEvent::Started, this, &AMobaPlayerController::OnSpacePressed);
		EnhancedInputComponent->BindAction(Space_Action, ETriggerEvent::Completed, this, &AMobaPlayerController::OnSpaceReleased);
	}
}

void AMobaPlayerController::MoveToMouseCursor()
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			FVector2D MousePosition;
			if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
			{
				
				if (GetHUD() != nullptr && GetHUD()->GetHitBoxAtCoordinates(MousePosition, true))
				{
					return;
				}

				// MousePosition为客户端鼠标在屏幕上的位置，需要转换成世界坐标传给服务端(参考GetHitResultAtScreenPosition函数的实现)
				FVector WorldOrigin;
				FVector WorldDirection;
				if (UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection) == true)
				{
					VerifyMouseWorldPositionClickOnServer(WorldOrigin, WorldDirection);
				}
			}
		}
	}
}

void AMobaPlayerController::OnRightClickPressed()
{
	bMoveToMouseCursor = true;
	// 点击特效
	if(GetLocalRole() == ROLE_AutonomousProxy)
	{
		SpawnNavigateClickFX();
	}
}

void AMobaPlayerController::OnRightClickReleased()
{
	bMoveToMouseCursor = false;
}


void AMobaPlayerController::OnWPressed_Implementation()
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		MobaPawn->SkillAttack(ESkillKey::ESK_W, nullptr);
	}
}

void AMobaPlayerController::OnWReleased()
{
}

void AMobaPlayerController::OnEPressed_Implementation()
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		MobaPawn->SkillAttack(ESkillKey::ESK_E, nullptr);
	}
}

void AMobaPlayerController::OnEReleased()
{
}

void AMobaPlayerController::OnRPressed_Implementation()
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		MobaPawn->SkillAttack(ESkillKey::ESK_R, nullptr);
	}
}

void AMobaPlayerController::OnRReleased()
{
}

void AMobaPlayerController::OnFPressed_Implementation()
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		MobaPawn->SkillAttack(ESkillKey::ESK_F, nullptr);
	}
}

void AMobaPlayerController::OnFReleased()
{
}

void AMobaPlayerController::OnSpacePressed_Implementation()
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		MobaPawn->SkillAttack(ESkillKey::ESK_Space, nullptr);
	}
}

void AMobaPlayerController::OnSpaceReleased()
{
}

void AMobaPlayerController::SpawnNavigateClickFX()
{
	if(FXCursor)
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
		if(HitResult.bBlockingHit)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitResult.ImpactPoint, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
	}
}

bool AMobaPlayerController::VerifyMouseWorldPositionClickOnServer_Validate(const FVector& WorldOrigin,
                                                                           const FVector& WorldDirection)
{
	return true;
}

void AMobaPlayerController::VerifyMouseWorldPositionClickOnServer_Implementation(const FVector& WorldOrigin, const FVector& WorldDirection)
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
				AActor* HitActor = HitResult.GetActor();
				if(Cast<AMobaHero>(HitActor) != MobaPawn->GetControlledMobaHero()) // 禁止攻击自己
				{
					MobaPawn->CharacterMoveToTargetWithAttackOnServer(HitResult.ImpactPoint, Cast<APawn>(HitActor));
				}
				
				return;
			}
		}

		if(TracePos(ECC_Visibility))
		{
			if(HitResult.bBlockingHit)
			{
				MobaPawn->CharacterMoveToOnServer(HitResult.ImpactPoint);
				return;
			}
		}
	}

}
