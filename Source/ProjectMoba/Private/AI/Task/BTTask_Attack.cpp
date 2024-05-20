// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Attack.h"

#include "AI/MobaAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Character/MobaCharacter.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(Blackboard_Actor.SelectedKeyType = UBlackboardKeyType_Object::StaticClass())
	{
		if(UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
		{
			if(AMobaAIController* AIController = Cast<AMobaAIController>(OwnerComp.GetOwner()))
			{
				if(AMobaCharacter* Target = Cast<AMobaCharacter>(BlackboardComponent->GetValueAsObject(Blackboard_Actor.SelectedKeyName)))
				{
					AIController->NormalAttack(Target);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_Attack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if(UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		Blackboard_Actor.ResolveSelectedKey(*BBAsset);
	}
}
