// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MobaMoveToAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UBTTask_MobaMoveToAttackRange : public UBTTask_MoveTo
{
	GENERATED_BODY()

protected:
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
