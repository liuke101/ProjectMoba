#include "AI/MobaAIController.h"

#include "ThreadManage.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/MobaCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AMobaAIController::AMobaAIController()
{
	
}

void AMobaAIController::BeginPlay()
{
	Super::BeginPlay();
	
	//延迟运行行为树，否则 UUBTDecorator_MobaAttackRange::InitializeMemory 获取角色属性时崩溃
	GThread::GetCoroutines().BindUObject(0.2f, this, &AMobaAIController::InitMobaAIController);
}


void AMobaAIController::InitMobaAIController()
{
	checkf(BehaviorTree, TEXT("BehaviorTree为空，请在BP_AIController中配置"));
	RunBehaviorTree(BehaviorTree);
}

void AMobaAIController::NormalAttack()
{
	if(AMobaCharacter* MobaCharacter = Cast<AMobaCharacter>(GetPawn()))
	{
		MobaCharacter->NormalAttack(GetTarget());
	}
}

void AMobaAIController::SetTarget(AMobaCharacter* InTarget)
{
	if(GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject("Target", InTarget);
	}
}

AMobaCharacter* AMobaAIController::GetTarget() const
{
	if(GetBlackboardComponent())
	{
		return Cast<AMobaCharacter>(GetBlackboardComponent()->GetValueAsObject("Target"));
	}
	return nullptr;
}

AMobaCharacter* AMobaAIController::GetTaskTarget()
{
	return nullptr;
}

AMobaCharacter* AMobaAIController::FindTarget()
{
	return nullptr;
}

void AMobaAIController::MoveToLocation(const FVector& Destination)
{
	SetTarget(nullptr);
	if(GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsVector("Location", Destination);
	}
}

FVector AMobaAIController::GetTargetLocation(AActor* RequestedBy) const
{
	if(GetBlackboardComponent())
	{
		return GetBlackboardComponent()->GetValueAsVector("Location");
	}
	return FVector::ZeroVector;
}

void AMobaAIController::StopAttackingRotate(bool bStopAttackRotate)
{
	if(GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsBool("bStopAttackRotate", bStopAttackRotate);
	}
}





