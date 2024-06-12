// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ProjectMoba/MobaType.h"
#include "CharacterSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API ACharacterSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ACharacterSpawnPoint();
public:
	FORCEINLINE ETeamType GetTeamType() const { return TeamType; }
	FORCEINLINE ECharacterType GetCharacterType() const { return CharacterType; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Type")
	ETeamType TeamType = ETeamType::ETT_Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Type")
	ECharacterType CharacterType = ECharacterType::ECT_None;

	
};
