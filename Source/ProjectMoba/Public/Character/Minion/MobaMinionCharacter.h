// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacter.h"
#include "MobaMinionCharacter.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaMinionCharacter : public AMobaCharacter
{
	GENERATED_BODY()

public:
	AMobaMinionCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		
public:
	virtual void Die() override;
	
};
