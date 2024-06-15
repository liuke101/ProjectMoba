// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaAIController.h"
#include "MobaPlayerAIController.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaPlayerAIController : public AMobaAIController
{
	GENERATED_BODY()

public:
	AMobaPlayerAIController();
	
	virtual void Tick(float DeltaTime) override;

public:
	virtual AMobaCharacter* FindTarget() override;

	virtual void MoveToLocation(const FVector& Destination) override;
protected:
	virtual void BeginPlay() override;

	bool bAutoFindTarget = true;

};
