// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Deorator/UBTDecorator_MobaCooldown.h"

#include "AI/MobaAIController.h"
#include "Table/CharacterAttribute.h"

bool UUBTDecorator_MobaCooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(AMobaAIController* OwnerAIController = OwnerComp.GetOwner<AMobaAIController>())
	{
		if(AMobaCharacter* MobaCharacter = OwnerAIController->GetPawn<AMobaCharacter>())
		{
			float* CDTptr = const_cast<float*>(&CoolDownTime); //去const（const函数不能直接修改成员变量）
			*CDTptr = 1 / MobaCharacter->GetCharacterAttribute()->AttackSpeed; // 攻速和时间的关系：Time = 1 / AttackSpeed
		}
	}
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);;
}
