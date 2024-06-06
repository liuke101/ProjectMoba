// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_SlotBase.h"
#include "UI_SimpleInventorySlot.generated.h"

struct FSlotData;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_SimpleInventorySlot : public UUI_SlotBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:
	void UpdateSlot(const FSlotData& SlotData) const;

private:
	//数量
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotNumber;
};
