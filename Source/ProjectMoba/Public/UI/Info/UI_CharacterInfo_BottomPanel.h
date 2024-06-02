﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_CharacterInfo_BottomPanel.generated.h"

class UProgressBar;
class UTextBlock;
class UUI_CharacterInfo;
class UUI_InventoryBar;
class UUI_SkillBar;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_CharacterInfo_BottomPanel : public UMobaUIBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_CharacterInfo> CharacterInfo; // 角色信息

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_SkillBar> SkillBar; // 技能栏

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_InventoryBar> InventoryBar; // 背包栏

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Level; // 等级

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GolbText; // 金币

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText; // 生命值

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ManaText; // 魔法值

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar; // 生命条

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ManaBar; // 魔法条

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ExpBar; // 经验条

public:
	virtual void NativeConstruct() override;
};
