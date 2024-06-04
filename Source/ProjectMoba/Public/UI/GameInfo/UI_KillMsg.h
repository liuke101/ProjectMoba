// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_KillMsg.generated.h"

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

private:
	
	//击杀者
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillerNameText;

	//被杀者
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KilledNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> KilledIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> KillerIcon;


};
