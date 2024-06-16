﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_ItemBase.h"
#include "UI_Item.generated.h"

struct FSlotAsset;
class UTextBlock;

DECLARE_DELEGATE_OneParam(FCallItemSynthesisDelegate, int32);

/** Item面板UI */
UCLASS()
class PROJECTMOBA_API UUI_Item : public UUI_ItemBase
{
	GENERATED_BODY()
public:
	FCallItemSynthesisDelegate CallItemSynthesisDelegate;
	
protected:
	virtual void NativeConstruct() override;

public:
	virtual void UpdateSlot(const FSlotAsset* SlotAsset) override;

protected:
	virtual void OnClickedWidget() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void BindToolTip() override;
public:
	void SetItemIntroduction(const FText&InContent);
	void SetItemName(const FName& InName);
	void SetItemGold(const int32 InGold);

	
private:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Gold;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemIntroduction;
};
