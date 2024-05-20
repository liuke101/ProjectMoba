#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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
	
protected:
	TWeakObjectPtr<AMobaCharacter> Target;

private:
	UPROPERTY()
	UBehaviorTree* BehaviorTree;
};
