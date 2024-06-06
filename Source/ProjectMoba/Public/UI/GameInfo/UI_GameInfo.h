// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_GameInfo.generated.h"

class UUI_KDAInfo;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_GameInfo : public UMobaUIBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 时间显示
	FString GetCurrentCount(float NewTimeCount);

	void UpdateTeamKillCount(int32 InFriendlyKillCount, int32 InEnemyKillCount) const;

private:
	//友军击杀数
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FriendlyKillCount;
	
	//敌方击杀数
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EnemyKillCount;

	//时间
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	//KDA信息
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_KDAInfo> KDAInfo;

};
