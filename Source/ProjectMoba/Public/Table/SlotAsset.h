// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "SlotAsset.generated.h"

class ABullet;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FSlotAsset : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FSlotAsset();
	
	UPROPERTY(EditDefaultsOnly, Category = "Slot Class")
	TObjectPtr<UTexture2D> SlotIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Class")
	TSubclassOf<ABullet> SlotClass;
};
