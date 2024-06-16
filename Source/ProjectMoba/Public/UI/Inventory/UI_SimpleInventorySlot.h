// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectMoba/MiscData.h"
#include "UI/UI_SlotBase.h"
#include "UI/Interface/UI_TipInterface.h"
#include "UI_SimpleInventorySlot.generated.h"

class UBorder;
struct FSlotData;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_SimpleInventorySlot : public UUI_SlotBase, public IUI_TipInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:
	void UpdateSlot(const FSlotData* SlotData);
	virtual void ResetSlot() override;

	virtual UUI_Tip* GetTip() override;
private:
	//数量
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotNumber;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> SlotBorder;
	
private:
	FSlotData ClientCacheSlotData;
};
