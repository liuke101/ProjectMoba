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
		//TODO:由于SkillBar和InventoryBar共用了委托，SlotIDs这种包含了Inventory（Inventory先进行了初始化）我已我们不使用SlotIDs参数，而是重新获取SkillSlotID。 可以考虑将委托分开
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
		// SkillSlot的技能等级点和升级按钮
		MobaPlayerState->UpdateSkillLevelDelegate.BindUObject(this, &UUI_SkillBar::UpdateSkillLevelUI);

		// 升级提示
		MobaPlayerState->ShowSkillLevelUpDelegate.BindUObject(this, &UUI_SkillBar::ShowSkillLevelUpUI);
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
			InSlot->GetClickButton()->SetIsEnabled(true);
			InSlot->CacheSlotAttributeUpdateLevel();
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
		}
		return true;
	});
}

void UUI_SkillBar::ShowSkillLevelUpUI(const TArray<int32>& SlotIDs)
{
	//先全部显示
	CallAllSlot<UUI_SkillSlot>([&](UUI_SkillSlot *InSlot)
	{
		InSlot->GetUpdateLevelButton()->SetVisibility(ESlateVisibility::Visible);
		InSlot->GetUpdateLevelButton()->SetIsEnabled(true);
		
		return true;
	});

	//关掉对应的button
	for(auto& SlotID : SlotIDs)
	{
		CallAllSlot<UUI_SkillSlot>([&](UUI_SkillSlot *InSlot)
		{
			if(InSlot->GetSlotID() == SlotID)
			{
				InSlot->GetUpdateLevelButton()->SetIsEnabled(false);
				
				return false;
			}
			return true;
		});
	}
}

UPanelWidget* UUI_SkillBar::GetSlotLayoutPanel()
{
	return SkillSlotArray;
}

