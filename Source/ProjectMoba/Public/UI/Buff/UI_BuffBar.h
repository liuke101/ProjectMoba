// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI/UI_Info.h"
#include "UI_BuffBar.generated.h"

class UUI_BuffSlot;
class UWrapBox;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_BuffBar : public UUI_Info
{
	GENERATED_BODY()
public:
	UUI_BuffBar();

	virtual void NativeConstruct() override;

	void UpdateCD(int32 InSlotID, float InMaxCD);
	void UpdateCD(int64 InPlayerID, int32 InSlotID, float InMaxCD) ;
	void RemoveCD(int64 InPlayerID, int32 InSlotID);

	void Clear() const;

	virtual void InitSlotLayout(const TArray<int32>& SlotIDs) override;
	
	virtual UPanelWidget* GetSlotLayoutPanel() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> BuffBox;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_BuffSlot> BuffSlotClass;

};
