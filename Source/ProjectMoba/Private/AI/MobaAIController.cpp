#include "AI/MobaAIController.h"

#include "ThreadManage.h"
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
		//延迟运行行为树，否则 UUBTDecorator_MobaAttackRange::InitializeMemory 获取角色属性时崩溃
		//失败的解决方法：在OnPossess中运行行为树，
		GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &AMobaAIController::InitMobaAIController,0.1f, false);
		//GThread::GetCoroutines().BindUObject(0.5f, this, &AMobaAIController::InitMobaAIController);
	}
}


void AMobaAIController::InitMobaAIController()
{
	if(InitTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(InitTimerHandle);
	}
	
	RunBehaviorTree(BehaviorTree);
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





