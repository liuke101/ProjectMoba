#include "AI/MobaAIController.h"

#include "Character/MobaCharacter.h"
#include "BehaviorTree/BehaviorTree.h"

AMobaAIController::AMobaAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_AI(TEXT("/Game/ProjectMoba/Character/AI/BT_AI"));
	BehaviorTree = BT_AI.Object;
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



