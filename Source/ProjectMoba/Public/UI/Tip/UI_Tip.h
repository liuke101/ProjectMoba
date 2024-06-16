// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Tip.generated.h"

class UTextBlock;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Tip : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetRichTextAction(const FText& InText) const;
	void SetRichTextActive(const FText& InText) const;
	void SetRichTextPassive(const FText& InText) const;
	void SetTextDescription(const FText& InText) const;
	void SetTextName(const FText& InText) const;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichTextAction; // 基础属性

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichTextActive; //主动效果

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichTextPassive; //被动效果

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextDescription; //描述

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextName; //描述
};
