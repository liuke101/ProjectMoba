// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MobaGameMode.generated.h"

class AMobaCharacter;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API AMobaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMobaGameMode();
	
	virtual void Tick(float DeltaSeconds) override;
	

protected:
	virtual void BeginPlay() override;

protected:
	void SpawnMinionsOnServer();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};


