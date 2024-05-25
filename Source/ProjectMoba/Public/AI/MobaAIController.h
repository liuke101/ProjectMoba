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
	
	void NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget);
	void SetTarget(AMobaCharacter* InTarget);
	FORCEINLINE AMobaCharacter* GetTarget() const { return Target.Get(); }
	
	virtual AMobaCharacter* FindTarget();
	void MoveToLocation(const FVector& Destination);
protected:
	TWeakObjectPtr<AMobaCharacter> Target;

private:
	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	UBehaviorTree* BehaviorTree = nullptr;

private:
	FTimerHandle InitTimerHandle;
};
