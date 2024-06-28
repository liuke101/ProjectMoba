// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Spawn/MobaSpawnAIComponent.h"

UMobaSpawnAIComponent::UMobaSpawnAIComponent()
	: CurrentLevel(1)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMobaSpawnAIComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMobaSpawnAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//随着时间升级
	if(CurrentLevel<=18)
	{
		if(GetWorld()->GetRealTimeSeconds() >= 60 * 3.0f) //三分钟
		{ 
			CurrentLevel = 4;
		}
		else if(GetWorld()->GetRealTimeSeconds() >= 60 * 6.0f)
		{
			CurrentLevel = 8;
		}
		else if(GetWorld()->GetRealTimeSeconds() >= 60 * 10.0f)
		{
			CurrentLevel = 12;
		}
		else if(GetWorld()->GetRealTimeSeconds() >= 60 * 15.0f)
		{
			CurrentLevel = 16;
		}
		else if(GetWorld()->GetRealTimeSeconds() >= 60 * 20.0f)
		{
			CurrentLevel = 18;
		}
	}
}

