// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Cooldown.h"
#include "UBTDecorator_MobaCooldown.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUBTDecorator_MobaCooldown : public UBTDecorator_Cooldown
{
	GENERATED_BODY()

	//cooldown控制攻速，随着等级提升，放在类似Tick的地方
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
