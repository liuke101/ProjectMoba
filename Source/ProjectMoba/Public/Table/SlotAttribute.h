// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "SlotAttribute.generated.h"

USTRUCT(BlueprintType)
struct FSlotAttribute : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FSlotAttribute();

	// UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	// float Health = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	float CD;
};
