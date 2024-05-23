// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "UBTDecorator_MobaAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUBTDecorator_MobaAttackRange : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
	
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	
};
