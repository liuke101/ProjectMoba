// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_Slot.h"
#include "UI_InventorySlot.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_InventorySlot : public UUI_Slot
{
	GENERATED_BODY()

	friend class UUI_Inventory;

public:
	UUI_InventorySlot();

	virtual void NativeConstruct() override;
	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnClickedWidget() override;

	//数据相关
public:
	//如果是消费物品, 更新数量
	void UpdateNumber();
	//更新对应的Slot(服务器通知客户端更新指定的Slot)
	virtual void UpdateSlot() override;
	//开始进入更新CD的状态（服务器计算CD)
	virtual void StartUpdateCD() override;
	//结束更新CD (服务器更新结束后通知客户端，客户端矫正CD)
	virtual void EndUpdateCD() override;
	//重置Slot
	virtual void ResetSlot() override;
	//获取Slot数量
	int32 GetSlotNumber();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tip)
	UWidget* GetInventoryTip();

protected:
	//操作
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	//数量
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotNumber;

	//拖拽显示图标类
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_IconDragDrop> ICODragDrogClass;

	//提示类
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_InventoryTip> InventoryTipClass;
	
	bool bDrag = false;
};
