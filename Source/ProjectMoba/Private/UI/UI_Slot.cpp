// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_Slot.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Game/MobaPlayerState.h"

void UUI_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 绑定点击事件
	ClickButton->OnClicked.AddDynamic(this, &UUI_Slot::OnClickedWidget);
}

void UUI_Slot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(MobaPlayerState)
	{
		//客户端更新CD
		if(FSlotData* SlotData = MobaPlayerState->GetSlotData(GetSlotID()))
		{
			if(SlotData->CD > 0.0f)
			{
				SlotData->CD -= InDeltaTime;
				DrawSlotCDMat(SlotData->CD / BuildSlot.MaxCD); //注意这里要除MaxCD
				DrawSlotCDText(SlotData->CD);
			
				if(SlotData->CD <= 0.0f)
				{
					DrawSlotCDMat(0.0f);
					DrawSlotCDText(0.0f);
				}
			}
		}
	}
	
}

void UUI_Slot::ResetSlot()
{
	Super::ResetSlot();
	
	SlotCDValue->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Slot::UpdateSlot()
{
	if(MobaPlayerState)
	{
		if(const FSlotData* SlotData = MobaPlayerState->GetSlotData(GetSlotID()))
		{
			if(SlotData->DataID != INDEX_NONE)
			{
				UpdateIcon(SlotData->SlotIcon);
				DrawSlotCDMat(SlotData->CD);
				DrawSlotCDText(SlotData->CD);
			}
			else if(SlotData->DataID == INDEX_NONE)
			{
				ResetSlot(); 
			}
		}
		else
		{
			ResetSlot(); 
		}
	}
}

void UUI_Slot::StartUpdateCD()
{
	if(MobaPlayerState)
	{
		if(const FSlotData* SlotData = MobaPlayerState->GetSlotData(GetSlotID()))
		{
			BuildSlot.MaxCD = SlotData->CD;
		}
	}
}

void UUI_Slot::EndUpdateCD()
{
	if(MobaPlayerState)
	{
		if(FSlotData* SlotData = MobaPlayerState->GetSlotData(GetSlotID()))
		{
			UpdateIcon(SlotData->SlotIcon);
			DrawSlotCDMat(SlotData->CD);
			DrawSlotCDText(SlotData->CD);
		}
	}
}

void UUI_Slot::SetKeyName(const FString& NewKeyName) const
{
	SlotKey->SetText(FText::FromString(NewKeyName));
}

void UUI_Slot::DrawSlotCDText(float CD) const
{
	SetTextNumber(SlotCDValue, CD);
}

void UUI_Slot::OnClickedWidget()
{
	
}

void UUI_Slot::OnReleasedWidget()
{
}
