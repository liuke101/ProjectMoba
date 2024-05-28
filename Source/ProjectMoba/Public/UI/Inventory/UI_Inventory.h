// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_Inventory.generated.h"



class UUniformGridPanel;
class UUI_InventorySlot;

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Inventory : public UMobaUIBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> SlotArrayInventory;

	UPROPERTY(EditDefaultsOnly,Category = UI)
	TSubclassOf<UUI_InventorySlot> InventorySlotClass;

protected:
	virtual void NativeConstruct() override;

	void LayoutInventroySlot(int32 ColumNumber, int32 RowNumber) const;

	// 呼叫所有的Slot
	void CallInventorySlotBreak(TFunction<bool(UUI_InventorySlot*)> InventorySlotLamada) const;
};
