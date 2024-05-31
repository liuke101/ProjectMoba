// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_ItemBase.h"
#include "UI_Item.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Item : public UUI_ItemBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void OnClickedWidget();
public:
	void SetItemIntroduction(const FText&InContent);
	void SetItemName(const FString& InName);
	void SetItemGold(const int32 InGold);

private:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Gold;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemIntroduction;
};
