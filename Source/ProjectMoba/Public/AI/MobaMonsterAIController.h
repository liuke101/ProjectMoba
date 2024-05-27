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

	
	FVector HomePoint = FVector::ZeroVector;
};
