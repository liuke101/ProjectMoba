// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Drop/UI_NativeOnDrop.h"
#include "UI_ItemSynthesis.generated.h"

class UUI_ItemSlot;
class UCanvasPanel;
struct FSlotAsset;

/** 绘制物品合成线 */
USTRUCT(BlueprintType)
struct FDrawEquipLine
{
	GENERATED_USTRUCT_BODY()

	// | 
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* VerticalLine;

	//！！
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* HorizontalLine;

	//_|_
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* TurnLeftAndRightVertically;

	//|-
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* TurnRight;

	//-|
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* TurnLeft;

	//-|-
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* TLine;
};

/** 物品合成UI */
UCLASS()
class PROJECTMOBA_API UUI_ItemSynthesis : public UUI_NativeOnDrop
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ItemCanvasPanel;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_ItemSlot> ItemSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FDrawEquipLine LineTexture;
public:
	virtual void NativeConstruct() override;

	void UpdateSlot(const FSlotAsset* SlotAsset);
};
