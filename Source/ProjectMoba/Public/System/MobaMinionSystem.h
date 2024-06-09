// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MobaMinionSystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaMinionSystem : public UObject
{
	GENERATED_BODY()

public:
	UMobaMinionSystem();

	
	void SpawnWarriorMinion();
	void SpawnRemoteMinion();
	void SpawnBigMinion();
	void SpawnSuperMinion();

private:
	float Time;
	uint8 Count;

	TArray<FVector> RedLocations;
	TArray<FVector> BlueLocations;
};
