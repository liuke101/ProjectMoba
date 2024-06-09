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
	virtual void InitMobaAIController() override;
	
	virtual AMobaCharacter* FindTarget() override;
	virtual AMobaCharacter* GetTaskTarget() override;
	
	void FindTaskTarget(const TArray<AActor*>& OutActors);
private:
	UPROPERTY()
	TObjectPtr<AMobaCharacter> TaskTarget;;
};
