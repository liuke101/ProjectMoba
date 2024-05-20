// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MobaAttack.h"

#include "AI/MobaAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Character/MobaCharacter.h"

EBTNodeResult::Type UBTTask_MobaAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(Blackboard_Target.SelectedKeyType = UBlackboardKeyType_Object::StaticClass())
	{
		if(UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
		{
			if(AMobaAIController* AIController = Cast<AMobaAIController>(OwnerComp.GetOwner()))
			{
				if(AMobaCharacter* Target = Cast<AMobaCharacter>(BlackboardComponent->GetValueAsObject(Blackboard_Target.SelectedKeyName)))
				{
					AIController->NormalAttack(Target);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_MobaAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if(UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		Blackboard_Target.ResolveSelectedKey(*BBAsset);
	}
}
