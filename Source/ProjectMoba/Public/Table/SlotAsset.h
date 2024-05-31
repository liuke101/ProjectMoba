// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "ProjectMoba/MobaType.h"
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

	// UPROPERTY(EditDefaultsOnly, Category = "Slot Asset")
	// TSubclassOf<ABullet> SlotClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Slot Asset")
	FName SlotName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Slot Asset")
	TObjectPtr<UTexture2D> SlotIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Asset")
	int32 SlotGold;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Asset")
	FText SlotIntroduction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Slot Asset")
	TArray<ESlotType> SlotType;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Asset")
	TArray<int32> ChildrensDataID; //合成所需的子物品
};
