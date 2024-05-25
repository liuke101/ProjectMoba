// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/BTTask_MobaMoveToAttackRange.h"
#include "AI/MobaAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "Table/CharacterAttribute.h"


void UBTTask_MobaMoveToAttackRange::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
}

EBTNodeResult::Type UBTTask_MobaMoveToAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	bObserveBlackboardValue = true; //允许中断移动
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}


