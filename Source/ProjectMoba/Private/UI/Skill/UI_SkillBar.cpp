// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/UI_SkillBar.h"

#include "ThreadManage.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "UI/Skill/UI_SkillSlot.h"

class UHorizontalBoxSlot;

void UUI_SkillBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_SkillBar::InitSlotLayout() const
{
	if (SkillSlotClass)
	{
		//1*4
		for (int32 x = 0; x < Layout_Row; x++)
		{
			for (int32 y = 0; y < Layout_Col; y++)  
			{
				if (UUI_SkillSlot* SlotWidget = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass))
				{
					if (UHorizontalBoxSlot* GridSlot = SkillSlotArray->AddChildToHorizontalBox(SlotWidget))
					{
						GridSlot->SetHorizontalAlignment(HAlign_Fill);
						GridSlot->SetVerticalAlignment(VAlign_Fill);
					}
					SlotWidget->SetSlotID(y); //设置SlotID

					//Update
					SlotWidget->UpdateSlot();
				}
			}
		}
	}
}

UPanelWidget* UUI_SkillBar::GetSlotLayoutPanel() const
{
	return SkillSlotArray;
}

