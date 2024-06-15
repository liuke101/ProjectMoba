// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_Tip.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Tip : public UMobaUIBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichTextAction;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichTextActive; //主动技能

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichTextPassive; //被动
};
