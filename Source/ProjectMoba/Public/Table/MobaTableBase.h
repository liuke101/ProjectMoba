// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable All
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MobaTableBase.generated.h"

USTRUCT(BlueprintType)
struct FMobaTableBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FMobaTableBase();

	UPROPERTY(EditDefaultsOnly, Category = "ID")
	int32 DataID;
};