// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService_MobaCharacter.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MobaMinion.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UBTService_MobaMinion : public UBTService_MobaCharacter
{
	GENERATED_BODY()
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};
