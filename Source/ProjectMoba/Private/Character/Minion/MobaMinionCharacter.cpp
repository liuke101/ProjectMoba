// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Minion/MobaMinionCharacter.h"


AMobaMinionCharacter::AMobaMinionCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMobaMinionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMobaMinionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobaMinionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMobaMinionCharacter::Die()
{
	Super::Die();
}

