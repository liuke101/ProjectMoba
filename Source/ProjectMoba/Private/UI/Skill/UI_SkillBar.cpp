// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/UI_SkillBar.h"

#include "ThreadManage.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Game/MobaPlayerState.h"
#include "UI/Skill/UI_SkillSlot.h"

class UHorizontalBoxSlot;

void UUI_SkillBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_SkillBar::InitSlotLayout() const
{
	// 防止重复初始化，因为该函数绑定在多播委托
	if(SkillSlotArray->GetChildrenCount() > 0) return;

	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		//获取所有SkillSlotID
		TArray<int32> SkillSlotIDs;
		MobaPlayerState->GetAllSkillSlotIDs(SkillSlotIDs);

		if(!SkillSlotIDs.IsEmpty())
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
								GridSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
								GridSlot->SetHorizontalAlignment(HAlign_Fill);
								GridSlot->SetVerticalAlignment(VAlign_Fill);
							}
							SlotWidget->SetSlotID(SkillSlotIDs[y]); //设置SlotID

							//Update
							SlotWidget->UpdateSlot();
						}
					}
				}
			}
		}
	}
}

UPanelWidget* UUI_SkillBar::GetSlotLayoutPanel() const
{
	return SkillSlotArray;
}

