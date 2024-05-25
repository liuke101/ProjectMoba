#pragma once

#include "CoreMinimal.h"
#include "MobaAIController.h"
#include "MobaMinionAIController.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaMinionAIController : public AMobaAIController
{
	GENERATED_BODY()

public:
	AMobaMinionAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual AMobaCharacter* FindTarget() override;
};
