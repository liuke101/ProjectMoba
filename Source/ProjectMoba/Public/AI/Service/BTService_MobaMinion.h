// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MobaMinion.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UBTService_MobaMinion : public UBTService
{
	GENERATED_BODY()
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Blackboard")
	FBlackboardKeySelector Blackboard_Target;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Blackboard")
	FBlackboardKeySelector Blackboard_Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Blackboard")
	FBlackboardKeySelector Blackboard_Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Blackboard")
	FBlackboardKeySelector Blackboard_Death;
};
