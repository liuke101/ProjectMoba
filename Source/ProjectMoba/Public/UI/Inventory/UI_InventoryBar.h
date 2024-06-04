// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_Bar.h"
#include "UI_InventoryBar.generated.h"



class UUniformGridPanel;
class UUI_InventorySlot;

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_InventoryBar : public UUI_Bar
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// 初始化Slot分布
	virtual void InitSlotLayout(const TArray<int32>& SlotIDs) override;

public:
	virtual UPanelWidget* GetSlotLayoutPanel() override;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventorySlotArray;

	//Slot组件
	UPROPERTY(EditDefaultsOnly,Category = "Moba|UI")
	TSubclassOf<UUI_InventorySlot> InventorySlotClass;

	
};
