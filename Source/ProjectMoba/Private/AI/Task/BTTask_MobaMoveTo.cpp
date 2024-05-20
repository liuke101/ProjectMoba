// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MobaMoveTo.h"

EBTNodeResult::Type UBTTask_MobaMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AcceptableRadius = 950.0f;
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_MobaMoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}
