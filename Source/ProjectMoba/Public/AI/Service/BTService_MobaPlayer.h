
#pragma once

#include "CoreMinimal.h"
#include "BTService_MobaCharacter.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MobaPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UBTService_MobaPlayer : public UBTService_MobaCharacter
{
	GENERATED_BODY()
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

};
