// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_Bar.h"

#include "ThreadManage.h"
#include "Game/MobaPlayerState.h"
#include "UI/UI_Slot.h"

void UUI_Bar::NativeConstruct()
{
	Super::NativeConstruct();
	
	//延迟绑定SlotDelegate, 因为PlayerState在NativeConstruct时还未初始化
	GThread::GetCoroutines().BindUObject(0.5f, this, &UUI_Bar::BindSlotDelegate);
}

void UUI_Bar::BindSlotDelegate()
{
	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		// 绑定初始化Slot分布
		MobaPlayerState->InitSlotDelegate.AddLambda([&](const TArray<int32>& InSlotIDs)
		{
			InitSlotLayout(InSlotIDs);  
		});

		// 绑定更新ID对应的Slot
		MobaPlayerState->UpdateSlotDelegate.AddLambda([&](int32 InSlotID)
		{
			CallAllSlot<UUI_Slot>([&](UUI_Slot* InSlot)
			{
				if(InSlot->GetSlotID() == InSlotID)
				{
					InSlot->UpdateSlot();
					return false; //如果找到停止Call
				}
				return true;
			});
		});

		// 绑定开始更新CD
		MobaPlayerState->StartUpdateCDSlotDelegate.AddLambda([&](const int32 InSlotID)
		{
			CallAllSlot<UUI_Slot>([&](UUI_Slot* InSlot)
			{
				if (InSlot->GetSlotID() == InSlotID)
				{
					InSlot->StartUpdateCD();
					return false;
				}
				return true;
			});
		});

		// 绑定停止更新CD
		MobaPlayerState->EndUpdateCDSlotDelegate.AddLambda([&](const int32 InSlotID)
		{
			CallAllSlot<UUI_Slot>([&](UUI_Slot* InSlot)
			{
				if (InSlot->GetSlotID() == InSlotID)
				{
					InSlot->EndUpdateCD();
					return false;
				}
				return true;
			});
		});
	}
}

