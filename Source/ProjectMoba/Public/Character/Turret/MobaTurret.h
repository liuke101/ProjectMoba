// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacter.h"
#include "MobaTurret.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaTurret : public AMobaCharacter
{
	GENERATED_BODY()

public:
	AMobaTurret();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FRotator TowersRotator;
};
