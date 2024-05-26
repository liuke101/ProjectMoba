// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Deorator/UBTDecorator_MobaAttackRange.h"

#include "AI/MobaAIController.h"
#include "Table/CharacterAttribute.h"

void UUBTDecorator_MobaAttackRange::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                 EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	if(AMobaAIController* OwnerAIController = OwnerComp.GetOwner<AMobaAIController>())
	{
		if(AMobaCharacter* MobaCharacter = OwnerAIController->GetPawn<AMobaCharacter>())
		{
			float* FVptr = const_cast<float*>(&FloatValue); //去const（const函数不能直接修改成员变量）
			
			*FVptr = MobaCharacter->GetCharacterAttribute()->AttackRange; 
		}
	}
}
