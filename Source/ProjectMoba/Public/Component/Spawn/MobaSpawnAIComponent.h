// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaSpawnActorComponent.h"
#include "MobaSpawnAIComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UMobaSpawnAIComponent : public UMobaSpawnActorComponent
{
	GENERATED_BODY()

public:
	UMobaSpawnAIComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }
	
private:
	int32 CurrentLevel;
};
