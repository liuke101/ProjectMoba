// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/BTTask_MobaMoveToAttackRange.h"
#include "AI/MobaAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "Table/CharacterAttribute.h"


void UBTTask_MobaMoveToAttackRange::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	/** 设置AcceptableRadius 略小于攻击范围 */
	if(AMobaAIController* OwnerAIController = OwnerComp.GetOwner<AMobaAIController>())
	{
		if(AMobaCharacter* MobaCharacter = OwnerAIController->GetPawn<AMobaCharacter>())
		{
			float* ARptr = const_cast<float*>(&AcceptableRadius); //去const（const函数不能直接修改成员变量）
			*ARptr = MobaCharacter->GetCharacterAttribute()->AttackRange - 20; //这个距离要略小于攻击范围，才能保证移动后进入攻击范围 
		}
	}

}

EBTNodeResult::Type UBTTask_MobaMoveToAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	bObserveBlackboardValue = true; //允许中断移动
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}


