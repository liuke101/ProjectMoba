// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MobaUIBase.h"
#include "Game/MobaPlayerState.h"
#include "Game/MobaGameState.h"
#include "UI/MobaHUD.h"

AMobaHUD* UMobaUIBase::GetMobaHUD() const
{
	return GetWorld()->GetFirstPlayerController()->GetHUD<AMobaHUD>();
}

AMobaPlayerState* UMobaUIBase::GetMobaPlayerState() const
{
	return GetWorld()->GetFirstPlayerController()->GetPlayerState<AMobaPlayerState>();
}

AMobaGameState* UMobaUIBase::GetMobaGameState() const
{
	return GetWorld()->GetGameState<AMobaGameState>();
}
