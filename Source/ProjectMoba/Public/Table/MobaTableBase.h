// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MobaTableBase.generated.h"

USTRUCT(BlueprintType)
struct FMobaTableBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FMobaTableBase();

	UPROPERTY(EditDefaultsOnly, Category = "Moba DataTable")
	int32 CharacterID;
};