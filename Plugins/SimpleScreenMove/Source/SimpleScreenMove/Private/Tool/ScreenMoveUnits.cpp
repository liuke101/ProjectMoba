// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "Tool/ScreenMoveUnits.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

bool FScreenMoveUnits::ListenScreenMove(APlayerController *PlayerController, float ScreenMoveSpeed /*= 100.f*/)
{
	return MoveDirection(PlayerController, CursorMove(PlayerController), ScreenMoveSpeed);
}

EScreenMoveState FScreenMoveUnits::CursorMove(const APlayerController *PlayerController)
{
	if (PlayerController)
	{
		/** 屏幕大小 */
		int32 SizeX = INDEX_NONE;
		int32 SizeY = INDEX_NONE;

		/** 鼠标位置 */
		float MousePostionX = INDEX_NONE;
		float MousePostionY = INDEX_NONE;
		
		PlayerController->GetViewportSize(SizeX, SizeY);
		PlayerController->GetMousePosition(MousePostionX, MousePostionY);

		/** 判断鼠标是否在屏幕内 */
		if (MousePostionX >= 0 && MousePostionX <= SizeX &&
			MousePostionY >= 0 && MousePostionY <= SizeY)
		{
			if (FMath::IsNearlyEqual(MousePostionX,0.0f,5.0f) && FMath::IsNearlyEqual(MousePostionY, 0.0f, 5.0f))
			{
				return EScreenMoveState::Screen_LeftAndUP; 
			}
			else if (FMath::IsNearlyEqual(MousePostionX, SizeX, 5.0f) && FMath::IsNearlyEqual(MousePostionY, SizeY, 5.0f))
			{
				return EScreenMoveState::Screen_RightAndDown;
			}
			else if (FMath::IsNearlyEqual(MousePostionX, SizeX, 5.0f) && FMath::IsNearlyEqual(MousePostionY, 0.0f, 5.0f))
			{
				return EScreenMoveState::Screen_RightAndUP;
			}
			else if (FMath::IsNearlyEqual(MousePostionX, 0.0f, 5.0f) && FMath::IsNearlyEqual(MousePostionY, SizeY, 5.0f))
			{
				return EScreenMoveState::Screen_LeftAndDown;
			}
			else if (FMath::IsNearlyEqual(MousePostionX, 0.0f, 5.0f))
			{
				return EScreenMoveState::Screen_Left;
			}
			else if (FMath::IsNearlyEqual(MousePostionY, 0.0f, 5.0f))
			{
				return EScreenMoveState::Screen_UP;
			}
			else if (FMath::IsNearlyEqual(MousePostionY, SizeY, 5.0f))
			{
				return EScreenMoveState::Screen_Down;
			}
			else if (FMath::IsNearlyEqual(MousePostionX, SizeX, 5.0f))
			{
				return EScreenMoveState::Screen_Right;
			}
		}
	}

	return EScreenMoveState::Screen_None;
}

bool FScreenMoveUnits::MoveDirection(APlayerController *PlayerController, EScreenMoveState ScreenMoveState, float ScreenMoveSpeed /*= 100.f*/)
{
	FVector OffsetValue = FVector::ZeroVector;

	if (PlayerController && PlayerController->GetPawn())
	{
		switch (ScreenMoveState)
		{
		case Screen_UP:
			OffsetValue = FVector(ScreenMoveSpeed, 0.0f, 0.0f);
			break;
		case Screen_Down:
			OffsetValue = FVector(-ScreenMoveSpeed, 0.0f, 0.0f);
			break;
		case Screen_Right:
			OffsetValue = FVector(0.0f, ScreenMoveSpeed, 0.0f);
			break;
		case Screen_Left:
			OffsetValue = FVector(0.0f, -ScreenMoveSpeed, 0.0f);
			break;
		case Screen_RightAndUP:
			OffsetValue = FVector(ScreenMoveSpeed, ScreenMoveSpeed, 0.0f);
			break;
		case Screen_RightAndDown:
			OffsetValue = FVector(-ScreenMoveSpeed, ScreenMoveSpeed, 0.0f);
			break;
		case Screen_LeftAndUP:
			OffsetValue = FVector(ScreenMoveSpeed, -ScreenMoveSpeed, 0.0f);
			break;
		case Screen_LeftAndDown:
			OffsetValue = FVector(-ScreenMoveSpeed, -ScreenMoveSpeed, 0.0f);
			break;
		default:
			break;
		}

		PlayerController->GetPawn()->AddActorWorldOffset(OffsetValue);
	}
	return OffsetValue != FVector::ZeroVector;
}

