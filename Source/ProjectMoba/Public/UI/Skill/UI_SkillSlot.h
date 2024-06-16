// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Table/SlotAttribute.h"
#include "UI/UI_Slot.h"
#include "UI/Interface/UI_TipInterface.h"
#include "UI_SkillSlot.generated.h"

struct FSkillLevelUpNetPackage;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_SkillSlot : public UUI_Slot, public IUI_TipInterface
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void OnClickedWidget() override;
	virtual void OnReleasedWidget() override;

	FORCEINLINE UProgressBar* GetSkillLevelBar() const { return SKillLevelBar; }
	FORCEINLINE UButton* GetUpdateLevelButton() const { return UpdateLevelButton; }

	virtual void UpdateSlot() override;

	virtual UUI_Tip* GetTip() override; //BUG 不显示技能信息

	//更新CacheSlotAttribute的等级
	void CacheSlotAttributeUpdateLevel();
protected:
	
	UFUNCTION()
	void OnClickedUpdateLevelButton();

	void InitSkillCache();
	
	
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> UpdateLevelButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> SKillLevelBar;
	
private:
	UPROPERTY()
	FSlotAttribute CacheSlotAttribute;
	
};
