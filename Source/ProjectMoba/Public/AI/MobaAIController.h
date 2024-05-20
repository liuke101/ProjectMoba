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
	void NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget);
	void SetTarget(AMobaCharacter* InTarget);
	AMobaCharacter* FindTarget();
	void MoveToLocation(const FVector& Destination);
protected:
	TWeakObjectPtr<AMobaCharacter> Target;

private:
	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	UBehaviorTree* BehaviorTree = nullptr;
};
