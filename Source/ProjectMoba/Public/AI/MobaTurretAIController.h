// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaAIController.h"
#include "MobaTurretAIController.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaTurretAIController : public AMobaAIController
{
	GENERATED_BODY()

public:
	AMobaTurretAIController();

protected:
	virtual void BeginPlay() override;

	
public:
	virtual void InitMobaAIController() override;
	
	virtual AMobaCharacter* FindTarget() override;
};
