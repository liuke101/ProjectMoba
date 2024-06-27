// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaSpawnActorComponent.h"
#include "Components/ActorComponent.h"
#include "MobaSpawnPlayerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UMobaSpawnPlayerComponent : public UMobaSpawnActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMobaSpawnPlayerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
