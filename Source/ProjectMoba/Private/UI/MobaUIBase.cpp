// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MobaUIBase.h"

#include "UI/MobaHUD.h"

AMobaHUD* UMobaUIBase::GetMobaHUD() const
{
	return Cast<AMobaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}
