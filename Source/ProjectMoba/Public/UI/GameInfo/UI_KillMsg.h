﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_KillMsg.generated.h"

struct FKillNetPackgae;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_KillMsg : public UMobaUIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void UpdateSlot();

protected:
	//普通击杀 
	void NormalKill(const FKillNetPackgae& KillNetPackgae, const FText& KillType);
	
	//连续击杀
	void ContinuousKill(const FKillNetPackgae& KillNetPackgae, const FText& KillType);

	//累计击杀
	void AccumulatedKill(const FKillNetPackgae& KillNetPackgae, const FText& KillType);

private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillTypeText;
	
	//击杀者
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillerNameText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> KillerIcon;
	
	//被杀者
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KilledNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> KilledIcon;

	


};
