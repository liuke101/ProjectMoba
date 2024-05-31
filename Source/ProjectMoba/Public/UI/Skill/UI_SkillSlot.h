// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_Slot.h"
#include "UI_SkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_SkillSlot : public UUI_Slot
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void OnClickedWidget() override;
	virtual void OnReleasedWidget() override;

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
