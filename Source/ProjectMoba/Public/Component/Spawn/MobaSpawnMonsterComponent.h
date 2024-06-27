// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaSpawnActorComponent.h"
#include "MobaSpawnMonsterComponent.generated.h"


class ACharacterSpawnPoint;
class AMobaMonsterCharacter;

USTRUCT()
struct FMonster
{
	GENERATED_BODY()
	FMonster();
	
	FVector SpawnPoint;
	TWeakObjectPtr<AMobaCharacter> MonsterPtr;
};

USTRUCT()
struct FMonsterGroup
{
	GENERATED_BODY()

	FMonsterGroup();

	ECharacterType CharacterType;
	bool bDieGroup; //组内怪物是否全死了
	TArray<FMonster> Monsters;

	float SpawnTime;
	float MaxSpawnTime;
};

/** 怪物生成器 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UMobaSpawnMonsterComponent : public UMobaSpawnActorComponent
{
	GENERATED_BODY()

public:
	UMobaSpawnMonsterComponent();

protected:
	virtual void BeginPlay() override;

	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints) override;
	void AllocationGroup(ACharacterSpawnPoint* SpawnPoint);
	
	void SpawnMonster(FMonsterGroup& MonsterGroup);
	

protected:
	TArray<FMonsterGroup> MonsterGroups;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Moba|Spawn")
	float GroupRange = 500.0f;
};
