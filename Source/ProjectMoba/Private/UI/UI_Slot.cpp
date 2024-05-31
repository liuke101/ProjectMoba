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
	ClickButton->OnClicked.AddDynamic(this, &UUI_Slot::OnClickedSlot);

	CDMaterialDynamic = UMaterialInstanceDynamic::Create(CDMaterialParent, this); //创建动态材质实例
	SlotCD->SetBrushFromMaterial(CDMaterialDynamic); //设置图片为动态材质
}

void UUI_Slot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//客户端更新CD
	if(FSlotData* SlotData = GetMobaPlayerState()->GetSlotData(GetSlotID()))
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

void UUI_Slot::ResetSlot()
{
	SlotIcon->SetVisibility(ESlateVisibility::Hidden);
	SlotCD->SetVisibility(ESlateVisibility::Hidden);
	SlotCDValue->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Slot::UpdateSlot()
{
	if(const FSlotData* SlotData = GetMobaPlayerState()->GetSlotData(GetSlotID()))
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
}

void UUI_Slot::StartUpdateCD()
{
	if(FSlotData* SlotData = GetMobaPlayerState()->GetSlotData(GetSlotID()))
	{
		BuildSlot.MaxCD = SlotData->CD;
	}
}

void UUI_Slot::EndUpdateCD()
{
	if(FSlotData* SlotData = GetMobaPlayerState()->GetSlotData(GetSlotID()))
	{
		UpdateIcon(SlotData->SlotIcon);
		DrawSlotCDMat(SlotData->CD);
		DrawSlotCDText(SlotData->CD);
	}
}

void UUI_Slot::SetKeyName(const FString& NewKeyName) const
{
	SlotKey->SetText(FText::FromString(NewKeyName));
}

void UUI_Slot::DrawSlotCDMat(float CD) const
{
	if (CDMaterialDynamic)
	{
		if (CD > 0.0f && CD < 1.0f)
		{
			CDMaterialDynamic->SetScalarParameterValue(SlotClearValueName, true);
			SlotCD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			CDMaterialDynamic->SetScalarParameterValue(SlotClearValueName, false);
			SlotCD->SetVisibility(ESlateVisibility::Hidden);
		}

		CDMaterialDynamic->SetScalarParameterValue(SlotMatCDName, CD);
	}
}

void UUI_Slot::DrawSlotCDText(float CD) const
{
	SetTextNumber(SlotCDValue, CD);
}

void UUI_Slot::UpdateIcon(UTexture2D* IconTexture) const
{
	if (SlotIcon)
	{
		if (IconTexture)
		{
			SlotIcon->SetBrushFromTexture(IconTexture);
			SlotIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SlotIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUI_Slot::OnClickedSlot()
{
	
}

void UUI_Slot::OnReleasedSlot()
{
}

void UUI_Slot::SetTextNumber(UTextBlock* TextNumberBlock, float TextNumber)
{
	if (TextNumber <= 0.f)
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		//0.0~1.0 保留一位小数
		if (TextNumber > 0.f && TextNumber <= 1.0f)
		{
			TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), TextNumber)));
		}
		// >1.0 只保留整数
		else
		{
			TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), static_cast<int32>(TextNumber))));
		}
		
		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_Slot::SetTextNumber(UTextBlock* TextNumberBlock, int32 TextNumber)
{
	if (TextNumber < 1)
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), TextNumber)));

		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
