// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_Info.h"
#include "UI_CharacterInfo_TopPanel.generated.h"

class UUniformGridPanel;
class UUI_CharacterInfo;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_CharacterInfo_TopPanel : public UUI_Info
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void BindDelegate() override;


	virtual UPanelWidget* GetSlotLayoutPanel() override;

private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_CharacterInfo> CharacterInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> SlotArrayInventory;

};
