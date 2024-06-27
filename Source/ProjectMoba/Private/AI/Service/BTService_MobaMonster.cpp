// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_MobaMonster.h"

void UBTService_MobaMonster::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
}

void UBTService_MobaMonster::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}
