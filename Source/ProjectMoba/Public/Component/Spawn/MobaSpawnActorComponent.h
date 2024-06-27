// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectMoba/MobaType.h"
#include "MobaSpawnActorComponent.generated.h"


class ACharacterSpawnPoint;
class AMobaCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UMobaSpawnActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMobaSpawnActorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints);
	
	AMobaCharacter* Spawn(int32 CharacterID, const FVector& Location, ETeamType TeamType) const;

	

	float CurrentLevel;
};
