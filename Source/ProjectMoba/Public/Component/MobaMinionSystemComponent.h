// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectMoba/MobaType.h"
#include "MobaMinionSystemComponent.generated.h"


class ACharacterSpawnPoint;
class AMobaCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UMobaMinionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMobaMinionSystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnWarriorMinion();
	void SpawnRemoteMinion();
	void SpawnBigMinion();
	void SpawnSuperMinion();

private:
	void SpawnMinion(int32 CharacterID, TArray<FVector>& Locations, ETeamType TeamType) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Moba|MinionSystem")
	float Time;
	uint8 MinionCount;

	TArray<FVector> RedLocations;
	TArray<FVector> BlueLocations;
};
