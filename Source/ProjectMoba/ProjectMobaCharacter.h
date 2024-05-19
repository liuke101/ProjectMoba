// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMobaCharacter.generated.h"

UCLASS(Blueprintable)
class AProjectMobaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectMobaCharacter();

	virtual void BeginPlay() override;
	
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	

};

