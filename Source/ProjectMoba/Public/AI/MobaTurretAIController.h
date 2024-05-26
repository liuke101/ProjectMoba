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

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void InitMobaAIController() override;

	virtual void SetTarget(AMobaCharacter* InTarget) override;
	virtual AMobaCharacter* FindTarget() override;

protected:
	bool bTargetInAttackRange = false; //范围内是否有目标
};
