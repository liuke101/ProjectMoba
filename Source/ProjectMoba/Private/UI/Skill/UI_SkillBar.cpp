// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/UI_SkillBar.h"

#include "ThreadManage.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "Game/MobaPlayerState.h"
#include "UI/Skill/UI_SkillSlot.h"

class UHorizontalBoxSlot;

void UUI_SkillBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_SkillBar::InitSlotLayout(const TArray<int32>& SlotIDs)
{
	// 防止重复初始化，因为该函数绑定在多播委托
	if(SkillSlotArray->GetChildrenCount() > 0) return;

	if(MobaPlayerState)
	{
		TArray<int32> IDs;
		MobaPlayerState->GetAllSkillSlotIDs(IDs);
		
		if(!IDs.IsEmpty())
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
							SlotWidget->SetSlotID(IDs[y]); //设置SlotID

							//Update
							SlotWidget->UpdateSlot();
						}
					}
				}
			}
		}
	}
}

void UUI_SkillBar::BindDelegate()
{
	Super::BindDelegate();

	if(MobaPlayerState)
	{
		MobaPlayerState->UpdateSkillLevelDelegate.BindUObject(this, &UUI_SkillBar::UpdateSkillLevelUI);
	}
	else
	{
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			BindDelegate();
		});
	}
}

void UUI_SkillBar::UpdateSkillLevelUI(const FSkillLevelUpNetPackage& InPackage)
{
	CallAllSlot<UUI_SkillSlot>([&](UUI_SkillSlot *InSlot)
	{
		//根据ID找到对应的Slot
		if(InSlot->GetSlotID() == InPackage.SlotID)
		{
			InSlot->GetSkillLevelBar()->SetPercent(static_cast<float>(InPackage.Level) / 3.0f);
			InSlot->GetUpdateLevelButton()->SetIsEnabled(InPackage.bEnableCurrentSlot);
			
			return false;
		}
		return true;
	});

	//隐藏所有button
	CallAllSlot<UUI_SkillSlot>([&](UUI_SkillSlot *InSlot)
	{
		if(InPackage.bHideAllSlot)
		{
			InSlot->GetUpdateLevelButton()->SetVisibility(ESlateVisibility::Hidden);
			return false;
		}
		return true;
	});
}

UPanelWidget* UUI_SkillBar::GetSlotLayoutPanel()
{
	return SkillSlotArray;
}

