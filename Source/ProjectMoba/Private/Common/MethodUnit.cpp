// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MethodUnit.h"

AMobaGameState* MethodUnit::GetMobaGameState(const UWorld* InWorld)
{
	if(InWorld)
	{
		return InWorld->GetGameState<AMobaGameState>();
	}
	return nullptr;
}


