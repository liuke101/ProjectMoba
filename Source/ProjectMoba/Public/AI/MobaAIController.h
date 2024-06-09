#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/MobaCharacter.h"
#include "MobaAIController.generated.h"

class AMobaCharacter;
class UBehaviorTree;

UCLASS()
class PROJECTMOBA_API AMobaAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMobaAIController();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void InitMobaAIController();
	
	void NormalAttack();
	
	virtual void SetTarget(AMobaCharacter* InTarget);
	virtual AMobaCharacter* GetTarget() const; //Target:范围内的目标，小兵、地方英雄、炮塔  
	virtual AMobaCharacter* GetTaskTarget(); //TaskTarget: 范围外的目标，最近的塔  最近的水晶
	virtual AMobaCharacter* FindTarget();
	
	void MoveToLocation(const FVector& Destination);

	void StopAttackingRotate(bool bStopAttackRotate);

private:
	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	UBehaviorTree* BehaviorTree = nullptr;
};
