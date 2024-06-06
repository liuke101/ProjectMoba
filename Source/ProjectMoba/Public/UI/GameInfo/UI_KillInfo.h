// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_KillInfo.generated.h"

struct FPlayerInfoNetPackage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_KillInfo : public UMobaUIBase
{
	GENERATED_BODY()

public:
	void UpdateSlot(const FPlayerInfoNetPackage& PlayerInfoNetPackage) const;
	
private:
	//击杀/死亡/助攻
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillInfoText;

	//补兵
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MinionKillNumber;

};
