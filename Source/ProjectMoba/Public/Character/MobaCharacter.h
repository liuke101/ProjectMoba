// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MobaCharacter.generated.h"

UCLASS(Blueprintable)
class AMobaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMobaCharacter();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget);

};

