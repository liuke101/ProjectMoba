// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_Bar.h"
#include "UI_SkillBar.generated.h"

class UUI_SkillSlot;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_SkillBar : public UUI_Bar
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	virtual void InitSlotLayout() const override;

public:
	virtual UPanelWidget* GetSlotLayoutPanel() const override;

private:
		
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SkillSlotArray;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	TSubclassOf<UUI_SkillSlot> SkillSlotClass;

};
