// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_ItemBase.h"
#include "UI_ItemSale.generated.h"

class UTextBlock;
/**
 * 物品出售UI
 */
UCLASS()
class PROJECTMOBA_API UUI_ItemSale : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void SetSellText(const FText& InContent);
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SellText;
};
