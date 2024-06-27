// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Spawn/MobaSpawnPlayerComponent.h"


UMobaSpawnPlayerComponent::UMobaSpawnPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMobaSpawnPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMobaSpawnPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

