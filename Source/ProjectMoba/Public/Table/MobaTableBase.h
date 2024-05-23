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

	/** CharacterID 工作流程： 1. 配表：每个CharacterID对应的行 2. 读txt获取当前的CharacterID */
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	int32 CharacterID;
};