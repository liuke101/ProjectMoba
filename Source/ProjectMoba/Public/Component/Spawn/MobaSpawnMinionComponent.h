// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaSpawnAIComponent.h"
#include "MobaSpawnMinionComponent.generated.h"


class ACharacterSpawnPoint;
class AMobaCharacter;

/** 小兵生成器 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UMobaSpawnMinionComponent : public UMobaSpawnAIComponent
{
	GENERATED_BODY()

public:
	UMobaSpawnMinionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints) override;
	
	void SpawnWarriorMinion();
	void SpawnRemoteMinion();
	void SpawnBigMinion();
	void SpawnSuperMinion();


private:
	UPROPERTY(EditDefaultsOnly, Category = "Moba|MinionSystem")
	float Time;
	uint8 MinionCount;

	TArray<FVector> RedLocations;
	TArray<FVector> BlueLocations;
};
