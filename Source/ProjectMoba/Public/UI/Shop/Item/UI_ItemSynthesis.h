// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Table/SlotAsset.h"
#include "UI/Drop/UI_NativeOnDrop.h"
#include "UI_ItemSynthesis.generated.h"

class UCanvasPanelSlot;
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
	UTexture2D* VerticalLine; //垂直线

	//！！
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* HorizontalLine; //水平线

	//-|-
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* TLine; //T型线
	
	//_|_
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* Re_TLine; //倒T型线

	//|-
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* TurnRight; //右拐角

	//-|
	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture2D* TurnLeft; //左拐角
};

struct FDrawSlot
{
	// 对应一个合成线
	struct FSlot
	{
		FSlot(){}
		
		FSlot(UCanvasPanelSlot* InSlot)
			: CanvasPanelSlot(InSlot)
		{}

		void SetPosition(const FVector2D& InPosition) const;
		FVector2D GetSize() const;

	private:
		// canvas slot设置
		TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot;
	};

	// 对应一个物品按钮
	struct FAsset : public FSlot
	{
		FAsset(): SlotAsset(nullptr)
		{
		}

		FAsset(UCanvasPanelSlot* InSlot, const FSlotAsset* InSlotAsset)
			: FSlot(InSlot)
			, SlotAsset(InSlotAsset)
		{}

		FORCEINLINE const FSlotAsset* GetSlotAsset() const { return SlotAsset; }

	private:
		const FSlotAsset* SlotAsset;
	};

	TArray<FSlot> LREndPanel; //拐弯先
	TArray<FSlot> VLinePanel; //垂直线
	TArray<FSlot> HLinePanel; //水平线
	TArray<FSlot> TLinePanel; //T型线
	TArray<FSlot> Re_TLinePanel; //倒T型线
	TArray<FAsset> ItemPanel; //物品
};

/** 物品合成UI(BUG) */
UCLASS()
class PROJECTMOBA_API UUI_ItemSynthesis : public UUI_NativeOnDrop
{
	GENERATED_BODY()

public:
	UUI_ItemSynthesis();
	
	virtual void NativeConstruct() override;

	void UpdateSlot(const FSlotAsset* SlotAsset);

protected:
	//递归创建Slot
	void RecursiveUpdateSlot(const FSlotAsset* SlotAsset, const FVector2D& InStartPosition, int8 SlotLayer);

	int32 GetLayerDepth(const FSlotAsset* SlotAsset, float InDepth = 1);
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ItemCanvasPanel;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	FVector2D StartPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	FVector2D IconSize;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	TSubclassOf<UUI_ItemSlot> ItemSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	FDrawEquipLine LineTexture;
};
