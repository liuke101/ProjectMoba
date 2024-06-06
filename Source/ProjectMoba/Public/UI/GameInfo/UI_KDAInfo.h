// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_KDAInfo.generated.h"

struct FPlayerKDANetPackage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_KDAInfo : public UMobaUIBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void UpdateSlot(const FPlayerKDANetPackage& PlayerKDANetPackage) const;
	
private:
	//KDA
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KDAInfoText;

	//补兵
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MinionKillNumber;

};
