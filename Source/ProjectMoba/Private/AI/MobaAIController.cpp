#include "AI/MobaAIController.h"

#include "Character/MobaCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AMobaAIController::AMobaAIController()
{
	
}

void AMobaAIController::BeginPlay()
{
	Super::BeginPlay();
	
	/** 仅在服务端运行行为树 */
	if(GetLocalRole()==ROLE_Authority)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AMobaAIController::NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget)
{
	Target = InTarget;
	if(AMobaCharacter* MobaCharacter = Cast<AMobaCharacter>(GetPawn()))
	{
		MobaCharacter->NormalAttack(Target);
	}
}

void AMobaAIController::SetTarget(AMobaCharacter* InTarget)
{
	GetBlackboardComponent()->SetValueAsObject("Target", InTarget);
	Target = InTarget;
}

AMobaCharacter* AMobaAIController::FindTarget()
{
	return nullptr;
}

void AMobaAIController::MoveToLocation(const FVector& Destination)
{
	SetTarget(nullptr);
	GetBlackboardComponent()->SetValueAsVector("Location", Destination);
}





