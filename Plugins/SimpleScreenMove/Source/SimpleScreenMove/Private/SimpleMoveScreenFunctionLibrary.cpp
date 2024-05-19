// Copyright (C) RenZhai.2023.All Rights Reserved.


#include "SimpleMoveScreenFunctionLibrary.h"
#include "Tool/ScreenMoveUnits.h"

bool USimpleMoveScreenFunctionLibrary::ListenScreenMove(UObject *WorldContextObject, float ScreenMoveSpeed /*= 100.f*/)
{
	if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{	
		return FScreenMoveUnits().ListenScreenMove(World->GetFirstPlayerController(), ScreenMoveSpeed);
	}

	return false;
}
