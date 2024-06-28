// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaSpawnActorComponent.h"
#include "Components/ActorComponent.h"
#include "MobaSpawnPlayerComponent.generated.h"

struct FPlayerCharacter
{
	FVector SpawnPointLocation = FVector::ZeroVector;
	AMobaCharacter* Character = nullptr;
	float Time = 10.0f;
	float MaxTime = 10.0f;

	void ResetDeathTime();
};

struct FPlayerCharacterGroup
{
	TArray<FPlayerCharacter> Characters;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UMobaSpawnPlayerComponent : public UMobaSpawnActorComponent
{
	GENERATED_BODY()

public:
	UMobaSpawnPlayerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void InitSpawnPoint(TArray<ACharacterSpawnPoint*> SpawnPoints) override;

public:
	AMobaCharacter* SpawnCharater(int64 PlayerID, int32 CharacterID, ETeamType TeamType);

	//在指定组中生成一个角色
	AMobaCharacter* SpawnCharacterInGroup(FPlayerCharacterGroup& Group, int64 PlayerID, int32 CharacterID, ETeamType TeamType);

	void CheakCroup(FPlayerCharacterGroup& Group, float DeltaTime);
	void ChangeTime(FPlayerCharacterGroup& Group, float InNewTime); //修改所有对象的时间

	
protected:
	FPlayerCharacterGroup RedGroup;
	FPlayerCharacterGroup BlueGroup;
};
