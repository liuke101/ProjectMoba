#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MobaPlayerState.generated.h"

class UPlayerDataComponent;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API AMobaPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMobaPlayerState();
	
	FORCEINLINE UPlayerDataComponent* GetPlayerDataComponent() const { return PlayerDataComponent; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|Component")
	TObjectPtr<UPlayerDataComponent> PlayerDataComponent;
};
