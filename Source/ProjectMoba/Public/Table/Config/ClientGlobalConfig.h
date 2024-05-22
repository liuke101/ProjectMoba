// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ClientGlobalConfig.generated.h"

/**
 *  客户端全局配置表
 */
USTRUCT()
struct FClientGlobalConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FClientGlobalConfig();

	// minimap大小
	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	float MinMiniMapSize;

	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	float MaxMiniMapSize;

	// minimap缩放
	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	float MinTargetArmLength;

	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	float MaxTargetArmLength;
	
};
