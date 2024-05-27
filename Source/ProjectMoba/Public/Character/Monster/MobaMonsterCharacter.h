// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacter.h"
#include "MobaMonsterCharacter.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaMonsterCharacter : public AMobaCharacter
{
	GENERATED_BODY()

public:
	AMobaMonsterCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void InitCharacter() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
