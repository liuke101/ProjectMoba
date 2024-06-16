// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_Slot.h"
#include "UI/Interface/UI_TipInterface.h"
#include "UI_InventorySlot.generated.h"

class UUI_Tip;
class UUI_IconDragDrop;
class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_InventorySlot : public UUI_Slot, public IUI_TipInterface
{
	GENERATED_BODY()

	friend class UUI_InventoryBar;

public:
	virtual void NativeConstruct() override;
	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnClickedWidget() override;

	//数据相关
public:
	//更新物品数量显示
	void UpdateNumber() const;
	//更新对应的Slot(服务器通知客户端更新指定的Slot)
	virtual void UpdateSlot() override;
	//开始进入更新CD的状态（服务器计算CD)
	virtual void StartUpdateCD() override;
	//结束更新CD (服务器更新结束后通知客户端，客户端矫正CD)
	virtual void EndUpdateCD() override;
	//隐藏Slot
	virtual void ResetSlot() override;
	//获取SlotData数量
	int32 GetSlotNumber() const;

	virtual UUI_Tip* GetTip() override;
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
	TSubclassOf<UUI_IconDragDrop> IconDragDrogClass;

	//提示类
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_Tip> InventoryTipClass;
	
	bool bDrag = false;
};
