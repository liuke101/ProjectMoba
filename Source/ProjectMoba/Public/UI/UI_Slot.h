// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_Slot.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UMaterialInstanceDynamic;

/**
 *  Slot基类
 */
UCLASS()
class PROJECTMOBA_API UUI_Slot : public UMobaUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	//重置Slot
	virtual void ResetSlot();
	//更新对应的Slot
	virtual void UpdateSlot();
	//开始进入更新CD的状态
	virtual void StartUpdateCD();
	//结束更新CD
	virtual void EndUpdateCD();

	//键位设置
	void SetKeyName(const FString& NewKeyName) const;
	//绘制CD材质
	void DrawSlotCDMat(float InSlotCD) const;
	//绘制CD文本
	void DrawSlotCDText(float InSlotCD);
	//绘制Icon
	void UpdateIcon(UTexture2D* InIcon) const;

	//点击事件回调
	UFUNCTION(BlueprintCallable)
	virtual void OnClickedWidget();

	//设置唯一ID (注：该类中的SlotID指的是InventoryID,即背包格子的ID，而不是用于获取DataTable数据的SlotID, 命名有些冲突，后期应该修改并区分）
	FORCEINLINE void SetSlotID(const int32 InID) { BuildSlot.SlotID = InID; }
	FORCEINLINE int32 GetSlotID() const { return BuildSlot.SlotID; }
	
protected:
	//显示 float字体
	static void DisplayNumber(UTextBlock* TextNumberBlock, float TextNumber);
	//显示 int字体
	static void DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber);
	
	struct FBuildSlot
	{
		FBuildSlot()
			:MaxCD(0.f)
			,SlotID(INDEX_NONE)
		{}
		float MaxCD;
		int32 SlotID;
	} BuildSlot;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotCD;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotCDValue;

	//键位
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ClickButton;

	//名字方便CD寻找
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SlotMatCDName;
	//名字方便CD寻找
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SlotClearValueName;

	//CD动态材质
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CDMaterialDynamic;
};
