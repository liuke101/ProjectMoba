// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_GameInfo.generated.h"

class UUI_KillInfo;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_GameInfo : public UMobaUIBase
{
	GENERATED_BODY()
	
	//敌人杀人数量
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NumberOfEnemyKilledText;

	//友军杀人数量
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NumberOfFriendlyKilledText;

	//友军杀人数量
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	//友军杀人数量
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_KillInfo> KillInfo;

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
