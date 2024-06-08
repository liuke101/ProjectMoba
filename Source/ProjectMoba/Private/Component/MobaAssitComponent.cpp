// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MobaAssitComponent.h"


UMobaAssitComponent::UMobaAssitComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UMobaAssitComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UMobaAssitComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

