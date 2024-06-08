// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/MobaPlayerController.h"

#include "GameFramework/Pawn.h"
#include "NiagaraFunctionLibrary.h"

#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/MobaCharacter.h"
#include "Character/MobaPawn.h"
#include "Character/Hero/MobaHeroCharacter.h"
#include "Game/MobaPlayerState.h"

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

	if(GetLocalRole() == ROLE_AutonomousProxy)
	{
		MouseLockedInViewport();
	}
}

void AMobaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(LeftClick_Action, ETriggerEvent::Started, this, &AMobaPlayerController::OnLeftClickPressed);
		EnhancedInputComponent->BindAction(LeftClick_Action, ETriggerEvent::Completed, this, &AMobaPlayerController::OnLeftClickReleased);
		EnhancedInputComponent->BindAction(RightClick_Action, ETriggerEvent::Started, this, &AMobaPlayerController::OnRightClickPressed);
		EnhancedInputComponent->BindAction(RightClick_Action, ETriggerEvent::Completed, this, &AMobaPlayerController::OnRightClickReleased);
	}
}

void AMobaPlayerController::MoveToMouseCursor()
{
	FVector WorldOrigin;
	FVector WorldDirection;
	if(GetClientWorldPosFromScreenPos(WorldOrigin,WorldDirection))
	{
		Server_VerifyMouseWorldPositionClick(WorldOrigin, WorldDirection);
	}
}

void AMobaPlayerController::OnLeftClickPressed()
{
	FVector WorldOrigin;
	FVector WorldDirection;
	if(GetClientWorldPosFromScreenPos(WorldOrigin,WorldDirection))
	{
		Server_GetClickTargetCharacterInfo(WorldOrigin, WorldDirection);
	}
}

void AMobaPlayerController::OnLeftClickReleased()
{
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

void AMobaPlayerController::MouseLockedInViewport()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
}

void AMobaPlayerController::SpawnNavigateClickFX() const
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

bool AMobaPlayerController::GetClientWorldPosFromScreenPos(FVector& WorldOrigin, FVector& WorldDirection) const
{
	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePosition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
		{
			
			if (GetHUD() != nullptr && GetHUD()->GetHitBoxAtCoordinates(MousePosition, true))
			{
				return false;
			}

			// MousePosition为客户端鼠标在屏幕上的位置，需要转换成世界坐标传给服务端(参考GetHitResultAtScreenPosition函数的实现)
			if (UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection) == true)
			{
				return true;
			}
		}
	}

	return false;
}

void AMobaPlayerController::Server_GetClickTargetCharacterInfo_Implementation(const FVector& WorldOrigin,
	const FVector& WorldDirection)
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), false);

		// 检测到角色就显示角色信息TopPanel
		if(GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_Character, CollisionQueryParams)) 
		{
			if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(HitResult.GetActor()))
			{
				if(TargetCharacter != MobaPawn->GetControlledMobaHero()) // 不检测自己
				{
					if(AMobaPlayerState* MobaPlayerState = Cast<AMobaPlayerState>(PlayerState))
					{
						MobaPlayerState->UpdateCharacterInfo(TargetCharacter->GetPlayerID());
					}
				}
			}
		}
		//如果没检测到就隐藏角色信息TopPanel
		else if(GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_Visibility, CollisionQueryParams))
		{
			if(AMobaPlayerState* MobaPlayerState = Cast<AMobaPlayerState>(PlayerState))
			{
				MobaPlayerState->Client_HideCharacterInfoTopPanel();
			}
		}
	}
}

void AMobaPlayerController::Server_VerifyMouseWorldPositionClick_Implementation(const FVector& WorldOrigin, const FVector& WorldDirection)
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), false);

		// 如果射线命中返回true
		auto TracePos = [&](ECollisionChannel EccChannel)->bool
		{
			return GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, EccChannel, CollisionQueryParams);
		};

		if(TracePos(ECC_Character)) // 检测到角色就移动攻击
		{
			AActor* HitActor = HitResult.GetActor();
			if(Cast<AMobaHeroCharacter>(HitActor) != MobaPawn->GetControlledMobaHero()) // 禁止攻击自己
			{
				MobaPawn->Server_CharacterMoveToTargetWithAttack(HitResult.ImpactPoint, Cast<APawn>(HitActor));
			}
			
			return;
		}

		if(TracePos(ECC_Visibility)) // 没有检测到就移动
		{
			MobaPawn->Server_CharacterMoveTo(HitResult.ImpactPoint);
			return;
		}
	}

}
