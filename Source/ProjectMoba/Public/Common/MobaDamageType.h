// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MobaDamageType.generated.h"

struct FSlotAttribute;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UMobaDamageType();

#pragma region payload
	const FSlotAttribute* SlotAttribute = nullptr;

#pragma endregion
};
