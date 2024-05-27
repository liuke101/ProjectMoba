#pragma once

#include "CoreMinimal.h"
#include "MobaAIController.h"
#include "MobaMonsterAIController.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaMonsterAIController : public AMobaAIController
{
	GENERATED_BODY()

public:
	AMobaMonsterAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual AMobaCharacter* FindTarget() override;
	void CheckRange();
	bool bHoming = false; //是否在回家过程中，直到回家结束才会再次寻找目标
	
	FVector HomePoint = FVector::ZeroVector;
};
